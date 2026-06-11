#include "Wheels.h"
#include "LiquidCrystal_I2C.h"
#include <IRremote.hpp>
#include <Servo.h>

// ===================== PINS =====================
#define IR_PIN    2
#define IN4_PIN   12   // Right Forward
#define IN3_PIN   4    // Right Back
#define ENB_PIN   5    // Right Speed (PWM)
#define IN2_PIN   7    // Left Forward  (moved from pin 2 to free IR pin)
#define IN1_PIN   8    // Left Back
#define ENA_PIN   6    // Left Speed (PWM)
#define TRIG_PIN  A2
#define ECHO_PIN  A3
#define SERVO_PIN 3

// ===================== SPRING MODEL =====================
// Hooke's law: F = -k * x,  x = distance - TARGET
// F is mapped to PWM speed (clamped to MIN..MAX).
// "Stiff spring" = no overshoot; achieved by generous deadband + linear damping.
#define SPRING_TARGET      100   // equilibrium distance in cm
#define SPRING_TOLERANCE   5     // deadband (±cm): inside this zone the car stops
#define SPRING_MIN_SPEED   80    // minimum PWM that actually moves the motors
#define SPRING_MAX_SPEED   220   // maximum PWM speed
#define SPRING_ERROR_MAX   100   // |error| (cm) at which max speed is applied

// ===================== IR CODES (NEC) =====================
#define IR_UP     0xE718FF00
#define IR_DOWN   0xAD52FF00
#define IR_LEFT   0xF708FF00
#define IR_RIGHT  0xA55AFF00
#define IR_OK     0xE31CFF00
#define IR_HASH   0xF20DFF00   // '#' → toggle spring mode
#define IR_1      0xBA45FF00
#define IR_2      0xB946FF00
#define IR_3      0xB847FF00
#define IR_4      0xBB44FF00
#define IR_REPEAT 0x0

// ===================== PIN UNLOCK =====================
const uint32_t PIN_SEQ[] = { IR_1, IR_2, IR_3, IR_4 };
const int PIN_LEN = 4;

// ===================== OBJECTS =====================
byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);
Wheels w(lcd);
Servo serwo;

// ===================== STATE =====================
bool springMode = false;

unsigned int DISTANCE_SONAR = 300;  // cm, default = far away

// movementType: 0=stop, 1=fwd, 2=back, 3=rotL, 4=rotR
char movementType  = 0;
char prevMovement  = -1;
uint8_t prevSpringSpeed = 0;  // speed used last time springControl ran

unsigned long lastIRTime        = 0;
unsigned long lastRepeatTime    = 0;
unsigned long lastDirectionTime = 0;
unsigned long lastSonarTime     = 0;
bool receivingRepeat = false;

const unsigned long IR_COOLDOWN     = 300;
const unsigned long REPEAT_TIMEOUT  = 500;
const unsigned long INITIAL_HOLD_MS = 600;
const unsigned long SONAR_PERIOD_MS = 50;

// ===================== DECLARATIONS =====================
void waitForPIN();
void updateSonar();
void updateIR();
void driveWheels();
void springControl();
void updateLCD();
unsigned int measureDistance();

// ===================== SETUP =====================
void setup() {
    lcd.init();
    lcd.backlight();
    lcd.clear();

    w.attach(IN4_PIN, IN3_PIN, ENB_PIN, IN2_PIN, IN1_PIN, ENA_PIN, 13);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW);

    serwo.attach(SERVO_PIN);
    serwo.write(110);
    delay(300);

    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    waitForPIN();
}

// ===================== LOOP =====================
void loop() {
    updateSonar();
    updateIR();

    if (springMode) {
        springControl();
    } else {
        driveWheels();
    }

    updateLCD();
}

// ===================== PIN UNLOCK =====================
// Blocks in setup() until the user enters PIN 1-2-3-4 and presses OK.
// The PIN itself is the main protection against IR interference from other remotes.
void waitForPIN() {
    int entered = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter PIN:");

    while (true) {
        if (!IrReceiver.decode()) continue;

        uint32_t code = IrReceiver.decodedIRData.decodedRawData;
        IrReceiver.resume();

        if (code == IR_REPEAT) continue;

        if (code == IR_OK) {
            if (entered == PIN_LEN) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Access OK!");
                delay(1000);
                lcd.clear();
                return;
            }
            entered = 0;
            lcd.setCursor(0, 1);
            lcd.print("Wrong PIN!      ");
            delay(800);
            lcd.setCursor(0, 1);
            lcd.print("                ");
        } else if (entered < PIN_LEN && code == PIN_SEQ[entered]) {
            entered++;
            lcd.setCursor(0, 1);
            for (int i = 0; i < entered; i++) lcd.print('*');
            for (int i = entered; i < PIN_LEN; i++) lcd.print(' ');
        } else {
            entered = 0;
            lcd.setCursor(0, 1);
            lcd.print("Wrong PIN!      ");
            delay(800);
            lcd.setCursor(0, 1);
            lcd.print("                ");
        }
    }
}

// ===================== SONAR =====================
void updateSonar() {
    if (millis() - lastSonarTime < SONAR_PERIOD_MS) return;
    lastSonarTime = millis();
    serwo.write(90);  // always look straight ahead
    DISTANCE_SONAR = measureDistance();
}

unsigned int measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    unsigned long tot = pulseIn(ECHO_PIN, HIGH, 25000UL);  // 25ms ≈ 215 cm range
    if (tot == 0) return 300;
    return (unsigned int)(tot / 58);
}

// ===================== SPRING CONTROL (Hooke's law) =====================
// x     = DISTANCE_SONAR - SPRING_TARGET   (displacement from equilibrium)
// F     = -k * x                           (restoring force)
// speed = map(|x|, deadband, max_err, MIN_SPEED, MAX_SPEED)
//
// When obstacle moves away  (x > 0): car speeds up toward it (chases)
// When obstacle moves closer (x < 0): car slows / reverses   (backs off)
// "Stiff" = no oscillation; the deadband prevents hunting near equilibrium.
void springControl() {
    int dist  = (int)DISTANCE_SONAR;
    int error = dist - SPRING_TARGET;  // positive = too far, negative = too close

    // Inside deadband: stop
    if (abs(error) <= SPRING_TOLERANCE) {
        if (movementType != 0) {
            movementType    = 0;
            prevSpringSpeed = 0;
            w.stop();
        }
        return;
    }

    // Map |error| → PWM speed (Hooke's linear law)
    uint8_t speed = (uint8_t)map(
        constrain(abs(error), SPRING_TOLERANCE, SPRING_ERROR_MAX),
        SPRING_TOLERANCE, SPRING_ERROR_MAX,
        SPRING_MIN_SPEED, SPRING_MAX_SPEED
    );

    char newMovement = (error > 0) ? 1 : 2;

    // Only send commands to motors when something actually changes
    if (newMovement != movementType || speed != prevSpringSpeed) {
        movementType    = newMovement;
        prevSpringSpeed = speed;
        w.setSpeed(speed);
        if (error > 0) {
            w.forward();
        } else {
            w.back();
        }
    }
}

// ===================== IR =====================
void updateIR() {
    // Stop car if button released (no REPEAT signal for REPEAT_TIMEOUT ms)
    if (millis() - lastRepeatTime > REPEAT_TIMEOUT) {
        receivingRepeat = false;
    }
    if (!springMode) {
        bool keepMoving = receivingRepeat || (millis() - lastDirectionTime < INITIAL_HOLD_MS);
        if (!keepMoving) movementType = 0;
    }

    if (!IrReceiver.decode()) return;

    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();

    if (code == IR_REPEAT) {
        receivingRepeat = true;
        lastRepeatTime  = millis();
        return;
    }

    if (millis() - lastIRTime < IR_COOLDOWN) return;
    lastIRTime = millis();

    // '#' toggles spring mode from any state
    if (code == IR_HASH) {
        springMode    = !springMode;
        movementType  = 0;
        prevMovement  = -1;
        prevSpringSpeed = 0;
        w.stop();
        return;
    }

    if (springMode) return;  // other keys disabled in spring mode

    switch (code) {
        case IR_UP:    movementType = 1; lastDirectionTime = millis(); break;
        case IR_DOWN:  movementType = 2; lastDirectionTime = millis(); break;
        case IR_LEFT:  movementType = 3; lastDirectionTime = millis(); break;
        case IR_RIGHT: movementType = 4; lastDirectionTime = millis(); break;
        case IR_OK:    movementType = 0;                               break;
    }
}

// ===================== MANUAL DRIVE =====================
// Only sends commands to motors when movementType actually changes —
// avoids redundant LCD writes from displayAnimation() inside Wheels methods.
void driveWheels() {
    if (movementType == prevMovement) return;
    prevMovement = movementType;

    switch (movementType) {
        case 0: w.stop();                                  break;
        case 1: w.setSpeed(200); w.forward();              break;
        case 2: w.setSpeed(200); w.back();                 break;
        case 3: w.setSpeed(200); w.rotateLeft();           break;
        case 4: w.setSpeed(200); w.rotateRight();          break;
    }
}

// ===================== LCD =====================
void updateLCD() {
    lcd.setCursor(0, 0);
    if (springMode) {
        lcd.print("SPR ");
        lcd.print(DISTANCE_SONAR);
        lcd.print("cm  ");
        lcd.setCursor(0, 1);
        int error = (int)DISTANCE_SONAR - SPRING_TARGET;
        lcd.print("e:");
        if (error >= 0) lcd.print('+');
        lcd.print(error);
        lcd.print("cm  ");
    } else {
        switch (movementType) {
            case 0: lcd.print("STOP"); break;
            case 1: lcd.print("FWD "); break;
            case 2: lcd.print("BACK"); break;
            case 3: lcd.print("L   "); break;
            case 4: lcd.print("R   "); break;
        }
        lcd.setCursor(0, 1);
        lcd.print("S:");
        lcd.print(DISTANCE_SONAR);
        lcd.print("cm   ");
    }
}
