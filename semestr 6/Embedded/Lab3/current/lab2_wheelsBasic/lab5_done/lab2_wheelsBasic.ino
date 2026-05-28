#include "Wheels.h"
#include "LiquidCrystal_I2C.h"
#include <Servo.h>

byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

int EnA = 6;
int EnB = 5;
int In1 = 8;
int In2 = 2;
int In3 = 4;
int In4 = 12;
int speaker_pin = 8;

#define TRIG      A2
#define ECHO      A3
#define SERVO_PIN 3

#define DRIVE_SPEED  200
#define STOP_DIST    30
#define TURN_DEG     60

Wheels w(lcd);
Servo serwo;

unsigned int measureDistance();
unsigned int lookAt(byte angle);
unsigned int measureAroundDrive();

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  w.attach(In4, In3, EnB, In2, In1, EnA, speaker_pin);
  w.setupSensors();
  w.goForward(30);


  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);

  w.attach(In4, In3, EnB, In2, In1, EnA, speaker_pin);
  w.setupSensors();
  w.goForward(30);

  serwo.attach(SERVO_PIN);
  serwo.write(90);   // patrz prosto
  delay(500);

  w.setSpeed(DRIVE_SPEED);
  w.forward();
}

void loop() {
  // unsigned int dist = measureDistance();
  unsigned int dist = measureAroundDrive();


  lcd.setCursor(0, 0);
  lcd.print("-> ");
  lcd.print(dist);
  lcd.print("cm    ");
  Serial.println(dist);

  if (dist > 0 && dist < STOP_DIST) {
    w.stop();
    lcd.clear();

    // skanuj lewo i prawo
    unsigned int dFarLeft  = lookAt(160);
    unsigned int dLeft  = lookAt(120);
    unsigned int dRight = lookAt(60);
    unsigned int dFarRight = lookAt(20);
    if (dFarLeft < dLeft) {
      dLeft = dFarLeft;
    }
    if (dFarRight < dRight) {
      dRight = dFarRight;
    }

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

    delay(100);

    if (dLeft >= dRight) {
      w.turnLeft(TURN_DEG);
      dist = measureDistance();

      while (dist < STOP_DIST) {
        dist = measureDistance();
        w.turnLeft(TURN_DEG);
      }
    } else {
      w.turnRight(TURN_DEG);
      dist = measureDistance();
      while (dist < STOP_DIST) {
        dist = measureDistance();
        w.turnRight(TURN_DEG); 
      }
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
  if (tot == 0) return 999;
  return (unsigned int)(tot / 58);
}

unsigned int lookAt(byte angle) {
  serwo.write(angle);
  delay(200);   // czekaj az serwo dotrze do pozycji
  return measureDistance();
}

unsigned int measureAroundDrive() {
  unsigned int rd = lookAt(60);
  unsigned int st = lookAt(90);
  unsigned int ld = lookAt(140);
  unsigned int min1;
  unsigned int min2;
  
  if (rd < st) {
    min1 = rd;
  }
  else {
    min1 = st;
  }
  if (min1 < ld) {
    min2 = min1;
  }
  else {
    min2 = ld;
  }

  return min2;

}


ISR(PCINT1_vect) {
  if (PINC & (1 << PC0)) Wheels::cnt0++;
  if (PINC & (1 << PC1)) Wheels::cnt1++;
}
