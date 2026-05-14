#include "Wheels.h"
#include "LiquidCrystal_I2C.h"
#include <Servo.h>

byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

int EnA = 5;
int EnB = 9;
int In1 = 6;
int In2 = 7;
int In3 = 10;
int In4 = 11;
int speaker_pin = 8;

#define TRIG      A4
#define ECHO      A5
#define SERVO_PIN 3

#define DRIVE_SPEED  150
#define STOP_DIST    20    // cm - zatrzymaj jesli przeszkoda blizej
#define TURN_DEG     60    // stopnie obrotu przy omijaniu

Wheels w(lcd);
Servo serwo;

unsigned int measureDistance();
unsigned int lookAt(byte angle);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);

  // w.attach() wywoluje Timer1.initialize() - musi byc przed serwo.attach()
  // aby Servo moglо pozniej przejac Timer1
  w.attach(In4, In3, EnB, In2, In1, EnA, speaker_pin);
  w.setupSensors();

  serwo.attach(SERVO_PIN);
  serwo.write(90);   // patrz prosto
  delay(500);

  w.setSpeed(DRIVE_SPEED);
  w.forward();
}

void loop() {
  unsigned int dist = measureDistance();

  lcd.setCursor(0, 0);
  lcd.print("-> ");
  lcd.print(dist);
  lcd.print("cm    ");
  Serial.println(dist);   // Kresłarka: odległość w czasie

  if (dist > 0 && dist < STOP_DIST) {
    w.stop();
    lcd.clear();

    // skanuj lewo i prawo
    unsigned int dLeft  = lookAt(30);
    unsigned int dRight = lookAt(150);
    serwo.write(90);
    delay(300);

    lcd.setCursor(0, 0);
    lcd.print("L:");
    lcd.print(dLeft);
    lcd.print("  R:");
    lcd.print(dRight);
    Serial.print("SCAN L:");
    Serial.print(dLeft);
    Serial.print(" R:");
    Serial.println(dRight);

    delay(400);

    if (dLeft >= dRight) {
      w.turnLeft(TURN_DEG);
    } else {
      w.turnRight(TURN_DEG);
    }

    lcd.clear();
    w.setSpeed(DRIVE_SPEED);
    w.forward();
  }

  delay(100);
}

unsigned int measureDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long tot = pulseIn(ECHO, HIGH, 30000UL);
  if (tot == 0) return 999;   // brak echa = brak przeszkody w zasięgu
  return (unsigned int)(tot / 58);
}

unsigned int lookAt(byte angle) {
  serwo.write(angle);
  delay(400);   // czekaj az serwo dotrze do pozycji
  return measureDistance();
}

ISR(PCINT1_vect) {
  if (PINC & (1 << PC0)) Wheels::cnt0++;
  if (PINC & (1 << PC1)) Wheels::cnt1++;
}
