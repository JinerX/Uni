#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

// LM393 sound sensor - digital output (DO)
// If claps register as LOW instead of HIGH, swap HIGH<->LOW in detectClaps().
#define SOUND_PIN  2

// Motor driver (L298) pins
#define IN4_PIN   12   // Right Forward
#define IN3_PIN   4    // Right Back
#define ENB_PIN   5    // Right Speed (PWM)
#define IN2_PIN   7    // Left Forward
#define IN1_PIN   8    // Left Back
#define ENA_PIN   6    // Left Speed (PWM)

// --- Tunable parameters ---
#define DRIVE_SPEED  200   // 0-255, motor PWM speed

// Minimum time between two claps to count them as separate (not bounce of one)
const unsigned long DEBOUNCE_MS  = 60;

// How long after the last clap to wait before evaluating the sequence.
// Increase if double/triple claps are being cut short.
// Decrease if there's too much lag before the car reacts.
const unsigned long EVAL_TIMEOUT = 700;
// --------------------------

byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);
Wheels w(lcd);

// Movement state: 0=stop, 1=forward, 2=rotateLeft, 3=rotateRight
char movementType = 1;
char prevMovement = -1;

int clapCount = 0;
unsigned long lastClapTime = 0;
int lastSoundState = LOW;

void detectClaps();
void applyMovement();
void updateLCD();

void setup() {
    Serial.begin(9600);
    Serial.println("=== Sound car ready ===");
    Serial.print("DEBOUNCE_MS=");  Serial.print(DEBOUNCE_MS);
    Serial.print("  EVAL_TIMEOUT="); Serial.print(EVAL_TIMEOUT);
    Serial.print("  DRIVE_SPEED=");  Serial.println(DRIVE_SPEED);

    lcd.init();
    lcd.backlight();
    lcd.clear();

    w.attach(IN4_PIN, IN3_PIN, ENB_PIN, IN2_PIN, IN1_PIN, ENA_PIN, 13);
    pinMode(SOUND_PIN, INPUT);
}

void loop() {
    detectClaps();
    applyMovement();
    updateLCD();
}

void detectClaps() {
    int currentState = digitalRead(SOUND_PIN);

    if (currentState == HIGH && lastSoundState == LOW) {
        unsigned long now = millis();
        if (now - lastClapTime > DEBOUNCE_MS) {
            clapCount++;
            lastClapTime = now;
            Serial.print("[CLAP] count=");
            Serial.print(clapCount);
            Serial.print("  t=");
            Serial.println(now);
        } else {
            Serial.print("[BOUNCE ignored] gap=");
            Serial.print(now - lastClapTime);
            Serial.println("ms");
        }
    }
    lastSoundState = currentState;

    if (clapCount > 0 && millis() - lastClapTime > EVAL_TIMEOUT) {
        Serial.print("[EVAL] claps=");
        Serial.print(clapCount);
        Serial.print(" -> ");

        switch (clapCount) {
            case 1:  movementType = 0; Serial.println("STOP");    break;
            case 2:  movementType = 2; Serial.println("LEFT");    break;
            case 3:  movementType = 3; Serial.println("RIGHT");   break;
            default: movementType = 1; Serial.println("FORWARD"); break;
        }
        clapCount = 0;
    }
}

void applyMovement() {
    if (movementType == prevMovement) return;
    prevMovement = movementType;

    Serial.print("[MOTOR] ");
    switch (movementType) {
        case 0: w.stop();                                  Serial.println("stop");         break;
        case 1: w.setSpeed(DRIVE_SPEED); w.forward();     Serial.println("forward");      break;
        case 2: w.setSpeed(DRIVE_SPEED); w.rotateLeft();  Serial.println("rotate left");  break;
        case 3: w.setSpeed(DRIVE_SPEED); w.rotateRight(); Serial.println("rotate right"); break;
    }
}

void updateLCD() {
    lcd.setCursor(0, 0);
    switch (movementType) {
        case 0: lcd.print("STOP"); break;
        case 1: lcd.print("FWD "); break;
        case 2: lcd.print("L   "); break;
        case 3: lcd.print("R   "); break;
    }
    lcd.setCursor(0, 1);
    if (clapCount > 0) {
        lcd.print("Claps: ");
        lcd.print(clapCount);
        lcd.print(" ");
    } else {
        lcd.print("                ");
    }
}
