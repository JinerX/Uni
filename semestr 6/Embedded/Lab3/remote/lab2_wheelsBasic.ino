#include "Wheels.h"
#include "LiquidCrystal_I2C.h"
#include <IRremote.hpp>

// IR sensor pin (must be pin 2 per lab instructions)
#define IR_PIN    2

// Motor driver (L298) pins.
// NOTE: In2 (Left Forward) moved from pin 2 to pin 7 to free up pin 2 for IR.
#define IN4_PIN   12   // Right Forward
#define IN3_PIN   4    // Right Back
#define ENB_PIN   5    // Right Speed (PWM)
#define IN2_PIN   7    // Left Forward
#define IN1_PIN   8    // Left Back
#define ENA_PIN   6    // Left Speed (PWM)

#define DRIVE_SPEED 200

// IR key codes (NEC protocol, 32-bit)
#define IR_UP     0xE718FF00
#define IR_DOWN   0xAD52FF00
#define IR_LEFT   0xF708FF00
#define IR_RIGHT  0xA55AFF00
#define IR_OK     0xE31CFF00
#define IR_1      0xBA45FF00
#define IR_2      0xB946FF00
#define IR_3      0xB847FF00
#define IR_4      0xBB44FF00
#define IR_REPEAT 0x0

// Startup PIN: press 1, 2, 3, 4 then confirm with OK.
// The PIN itself is the primary anti-interference mechanism — random codes
// from other remotes won't match the required sequence.
const uint32_t PIN_SEQ[] = { IR_1, IR_2, IR_3, IR_4 };
const int PIN_LEN = 4;

byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);
Wheels w(lcd);

// Movement state: 0=stop, 1=forward, 2=back, 3=rotateLeft, 4=rotateRight
char movementType = 0;
char prevMovement = -1;

unsigned long lastIRTime       = 0;
unsigned long lastRepeatTime   = 0;
unsigned long lastDirectionTime = 0;
bool receivingRepeat = false;

// Anti-interference: cooldown ignores rapid/spurious duplicate codes.
// Hold-to-move: car stops after button is released (repeat signal goes away).
const unsigned long IR_COOLDOWN     = 300;   // ms between processing new codes
const unsigned long REPEAT_TIMEOUT  = 500;   // ms before "button released" is declared
const unsigned long INITIAL_HOLD_MS = 600;   // ms to keep moving after initial press

void waitForPIN();
void updateIR();
void driveWheels();
void updateLCD();

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.clear();

    w.attach(IN4_PIN, IN3_PIN, ENB_PIN, IN2_PIN, IN1_PIN, ENA_PIN, 13);

    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

    waitForPIN();
}

void loop() {
    updateIR();
    driveWheels();
    updateLCD();
}

// Block in setup() until the correct PIN is entered and confirmed with OK.
void waitForPIN() {
    int entered = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter PIN:");

    while (true) {
        if (!IrReceiver.decode()) continue;

        uint32_t code = IrReceiver.decodedIRData.decodedRawData;
        IrReceiver.resume();

        if (code == IR_REPEAT) continue; // ignore hold-repeat during PIN entry

        if (code == IR_OK) {
            if (entered == PIN_LEN) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Access OK!");
                delay(1000);
                lcd.clear();
                return;
            }
            // Wrong or incomplete PIN — reset
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
            // Wrong digit — reset
            entered = 0;
            lcd.setCursor(0, 1);
            lcd.print("Wrong PIN!      ");
            delay(800);
            lcd.setCursor(0, 1);
            lcd.print("                ");
        }
    }
}

// Read IR remote and update movementType.
// Hold-to-move: direction keys require the button to be held (repeat signal).
// Anti-interference: short cooldown prevents the same code being processed twice quickly.
void updateIR() {
    // Car stops when both initial hold window and repeat signals have expired
    if (millis() - lastRepeatTime > REPEAT_TIMEOUT) {
        receivingRepeat = false;
    }
    bool keepMoving = receivingRepeat || (millis() - lastDirectionTime < INITIAL_HOLD_MS);
    if (!keepMoving) {
        movementType = 0;
    }

    if (!IrReceiver.decode()) return;

    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    IrReceiver.resume();

    if (code == IR_REPEAT) {
        receivingRepeat = true;
        lastRepeatTime = millis();
        return;
    }

    if (millis() - lastIRTime < IR_COOLDOWN) return;
    lastIRTime = millis();

    switch (code) {
        case IR_UP:    movementType = 1; lastDirectionTime = millis(); break;
        case IR_DOWN:  movementType = 2; lastDirectionTime = millis(); break;
        case IR_LEFT:  movementType = 3; lastDirectionTime = millis(); break;
        case IR_RIGHT: movementType = 4; lastDirectionTime = millis(); break;
        case IR_OK:    movementType = 0; break;
    }
}

// Apply movement only when state changes (avoids redundant motor/LCD writes).
void driveWheels() {
    if (movementType == prevMovement) return;
    prevMovement = movementType;

    switch (movementType) {
        case 0: w.stop();                               break;
        case 1: w.setSpeed(DRIVE_SPEED); w.forward();   break;
        case 2: w.setSpeed(DRIVE_SPEED); w.back();      break;
        case 3: w.setSpeed(DRIVE_SPEED); w.rotateLeft(); break;
        case 4: w.setSpeed(DRIVE_SPEED); w.rotateRight(); break;
    }
}

void updateLCD() {
    lcd.setCursor(0, 0);
    switch (movementType) {
        case 0: lcd.print("STOP"); break;
        case 1: lcd.print("FWD "); break;
        case 2: lcd.print("BACK"); break;
        case 3: lcd.print("L   "); break;
        case 4: lcd.print("R   "); break;
    }
}
