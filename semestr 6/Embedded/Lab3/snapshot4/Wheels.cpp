#include <TimerOne.h>

#include <Arduino.h>

#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)


Wheels* Wheels::instance = nullptr;
volatile uint16_t Wheels::cnt0 = 0;
volatile uint16_t Wheels::cnt1 = 0;

// ISR dla pinów A0 (PC0) i A1 (PC1) – zlicza impulsy enkoderów
ISR(PCINT1_vect) {
    if (PINC & (1 << PC0)) Wheels::cnt0++;
    if (PINC & (1 << PC1)) Wheels::cnt1++;
}

Wheels::Wheels(LiquidCrystal_I2C& lcd1) : lcd(lcd1){
    instance = this;
}

void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
    this->speed_right = s;
    this->displayAnimation();

}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
    this->speed_left = s;
    this->displayAnimation();

}

void Wheels::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
    this->displayAnimation();

}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS, int speaker_pin)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
    pinMode(speaker_pin, OUTPUT);
    this->speaker_pin = speaker_pin;
}

void Wheels::attachSensors(int sensorRight, int sensorLeft) {
    pinMode(sensorRight, INPUT);
    pinMode(sensorLeft, INPUT);
    PCICR  |= (1 << PCIE1);             // włącz grupę przerwań PCINT1 (PORTC)
    PCMSK1 |= (1 << PC0) | (1 << PC1); // monitoruj A0 i A1
}

void Wheels::attachSonar(int trig, int echo, int servoPin) {
    this->trigPin = trig;
    this->echoPin = echo;
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    serwo.attach(servoPin);
    serwo.write(90); // patrz przed siebie
    delay(500);
}

unsigned int Wheels::measureDistance(byte angle) {
    serwo.write(angle);
    delay(150); // czas na ruch serwa

    // standardowa sekwencja wyzwalania HC-SR04
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // pulseIn z timeoutem 30ms (max zasięg ~4m = ~24ms)
    unsigned long tot = pulseIn(echoPin, HIGH, 30000UL);
    if (tot == 0) return 400; // brak echa = bardzo daleko / brak obiektu

    // v_dźwięku = 340 m/s → 1 cm w 29 µs (tam+z powrotem: /58)
    return (unsigned int)(tot / 58);
}

int Wheels::scanAndDecide() {
    unsigned int dLeft  = measureDistance(150); // lewa burta ~150°
    unsigned int dRight = measureDistance(30);  // prawa burta ~30°
    serwo.write(90); // wróć na wprost

    lcd.setCursor(0, 0);
    lcd.print("L=");  lcd.print(dLeft);
    lcd.print(" R="); lcd.print(dRight);
    lcd.print("cm  ");

    Serial.print("Scan L="); Serial.print(dLeft);
    Serial.print(" R=");     Serial.println(dRight);

    return (dLeft >= dRight) ? -1 : 1; // -1=skręć lewo, +1=skręć prawo
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
    if (this->speed_left < 0) {
        this->speed_left = -(this->speed_left);
    }
    this->displayAnimation();
    Timer1.detachInterrupt();

}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
    if (this->speed_right < 0) {
        this->speed_right = -(this->speed_right);
    }
    this->displayAnimation();
    Timer1.detachInterrupt();

}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
    if (this->speed_left > 0) {
        this->speed_left = -(this->speed_left);
    }
    this->displayAnimation();

}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
    if (this->speed_right > 0) {
        this->speed_right = -(this->speed_right);
    }
    this->displayAnimation();

}

void Wheels::forward()
{
    this->forwardLeft();
    this->forwardRight();
    this->displayAnimation();
    Timer1.detachInterrupt();
    digitalWrite(this->speaker_pin, LOW);

}

void Wheels::back()
{
    this->backLeft();
    this->backRight();
    this->displayAnimation();
    this->TimerUpdate();
}

void Wheels::TimerUpdate() {
    int speed = abs(this->speed_left);
    if (speed < 60) speed = 60;
    // higher speed → fewer off-ticks → faster bip rate
    this->beepOffTicks = (uint16_t)map(speed, 60, 255, 20, 5);
    this->tickCount = 0;
    Timer1.detachInterrupt();
    Timer1.attachInterrupt(Wheels::makeSound, 50000UL); // 50ms per tick
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
    this->speed_left = 0;
    this->displayAnimation();

}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
    this->speed_right = 0;
    this->displayAnimation();

}

void Wheels::stop()
{
    this->stopLeft();
    this->stopRight();
    this->lcd.clear();
    digitalWrite(this->speaker_pin, LOW);
    this->displayAnimation();
    Timer1.detachInterrupt();
    digitalWrite(this->speaker_pin, LOW);

}



// Dodane

void Wheels::goForward(int cm) {
    uint16_t target = (uint16_t)cm * TICKS_PER_CM;
    cnt0 = 0; cnt1 = 0;
    unsigned long last_update = millis();

    this->setSpeed(200);
    this->forward();

    while (cnt0 < target || cnt1 < target) {
        unsigned long now = millis();
        if (now - last_update >= 200) {
            last_update = now;
            int progress_cm = max(cnt0, cnt1) / TICKS_PER_CM;
            int remaining = (progress_cm < cm) ? cm - progress_cm : 0;

            this->lcd.clear();
            this->lcd.setCursor(0, 1);
            this->lcd.print(remaining, DEC);
            this->displayAnimation();

            Serial.print("F cnt0="); Serial.print(cnt0);
            Serial.print(" cnt1="); Serial.println(cnt1);
        }
    }
    this->lcd.clear();
    this->stop();
}

void Wheels::goBack(int cm) {
    uint16_t target = (uint16_t)cm * TICKS_PER_CM;
    cnt0 = 0; cnt1 = 0;
    unsigned long last_update = millis();

    this->setSpeed(200);
    this->back(); // back() wywołuje TimerUpdate() → beeper aktywny

    while (cnt0 < target || cnt1 < target) {
        unsigned long now = millis();
        if (now - last_update >= 200) {
            last_update = now;
            int progress_cm = max(cnt0, cnt1) / TICKS_PER_CM;
            int remaining = (progress_cm < cm) ? cm - progress_cm : 0;

            this->lcd.clear();
            this->lcd.setCursor(0, 1);
            this->lcd.print(remaining, DEC);
            this->displayAnimation();

            Serial.print("B cnt0="); Serial.print(cnt0);
            Serial.print(" cnt1="); Serial.println(cnt1);
        }
    }
    this->lcd.clear();
    this->stop();
}


void Wheels::turnLeft(int degrees) {
    // pivot w miejscu: prawe do przodu, lewe do tyłu
    uint16_t ticks = (uint16_t)((uint32_t)degrees * 314UL * WHEEL_TRACK_CM * TICKS_PER_CM / 36000UL);
    cnt0 = 0; cnt1 = 0;
    this->setSpeed(150);
    this->forwardRight();
    this->backLeft();
    while (cnt0 < ticks || cnt1 < ticks) {}
    this->stop();
}

void Wheels::turnRight(int degrees) {
    // pivot w miejscu: lewe do przodu, prawe do tyłu
    uint16_t ticks = (uint16_t)((uint32_t)degrees * 314UL * WHEEL_TRACK_CM * TICKS_PER_CM / 36000UL);
    cnt0 = 0; cnt1 = 0;
    this->setSpeed(150);
    this->forwardLeft();
    this->backRight();
    while (cnt0 < ticks || cnt1 < ticks) {}
    this->stop();
}

void Wheels::displayAnimation() {
    this->lcd.setCursor(5,0);
    if (this->speed_right == 0) {
        this->lcd.print("RS");
    }
    else if (this->speed_right > 0) {
        this->lcd.print("RF");
    }
    else if (this->speed_right < 0) {
        this->lcd.print("RB");
    }
    this->lcd.setCursor(8,0);
    this->lcd.print("rV=");
    this->lcd.setCursor(11,0);
    this->lcd.print(this->speed_right,DEC);


    this->lcd.setCursor(5,1);
    if (this->speed_left == 0) {
        this->lcd.print("LS");
    }
    else if (this->speed_left > 0) {
        this->lcd.print("LF");
    }
    else if (this->speed_left < 0) {
        this->lcd.print("LB");
    }
    this->lcd.setCursor(8,1);
    this->lcd.print("lV=");
    this->lcd.setCursor(11,1);
    this->lcd.print(this->speed_left,DEC);

}

void Wheels::makeSound() {
    if (instance == nullptr) return;
    instance->tickCount++;
    uint16_t total = instance->beepOnTicks + instance->beepOffTicks;
    if (instance->tickCount >= total) instance->tickCount = 0;
    bool on = (instance->tickCount < instance->beepOnTicks);
    digitalWrite(instance->speaker_pin, on ? HIGH : LOW);
}


// void Wheels::startAnimation(LiquidCrystal_I2C lcd) {
//     unsigned long start_time = millis();
//     int counter = 0;
//     while (true) {
//         counter++;
//         if (counter % 17 == 0) {
//             lcd.setCursor(1,4);
//             lcd.print(67, DEC);
//         }
//         if (counter % 13 == 0) {
//             lcd.setCursor(1,4);
//             lcd.print(88, DEC);
//         }
//     }
// }


