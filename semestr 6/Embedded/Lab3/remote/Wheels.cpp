// #include <TimerOne.h>

#include <Arduino.h>

#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)


Wheels* Wheels::instance = nullptr;
volatile int Wheels::cnt0 = 0;
volatile int Wheels::cnt1 = 0;

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
    // this->TimerUpdate();
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
    pinMode(BEEPER, OUTPUT);
    // Timer1.initialize();
}

void Wheels::setupSensors() {
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    cnt0 = 0;
    cnt1 = 0;
    PCICR  = 0x02;
    PCMSK1 = 0x03;
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
    if (this->speed_left < 0) {
        this->speed_left = -(this->speed_left);
    }
    this->displayAnimation();
    // Timer1.detachInterrupt();

}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
    if (this->speed_right < 0) {
        this->speed_right = -(this->speed_right);
    }
    this->displayAnimation();
    // Timer1.detachInterrupt();

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
    this->goingBack = false;
    this->forwardLeft();
    this->forwardRight();
    this->displayAnimation();
    // Timer1.detachInterrupt();
    digitalWrite(BEEPER, LOW);

}

void Wheels::rotateLeft()
{
    this->backRight();
    this->forwardLeft();
    this->displayAnimation();
}

void Wheels::rotateRight()
{
    this->backLeft();
    this->forwardRight();
    this->displayAnimation();
}

void Wheels::back()
{
    this->goingBack = true;
    this->backLeft();
    this->backRight();
    this->displayAnimation();
    // this->TimerUpdate();
}

// void Wheels::TimerUpdate() {
//     if (!this->goingBack) return;
//     int speed = abs(this->speed_left);
//     unsigned long period;
//     if (speed >= 200) {
//         period = 200000UL;
//     } else if (speed >= 150) {
//         period = 300000UL;
//     } else if (speed >= 100) {
//         period = 400000UL;
//     } else {
//         period = 500000UL;
//     }
//     Timer1.detachInterrupt();
//     Timer1.attachInterrupt(Wheels::makeSound, period);
// }

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
    this->goingBack = false;
    this->stopLeft();
    this->stopRight();
    this->lcd.clear();
    this->displayAnimation();
    // Timer1.detachInterrupt();
    digitalWrite(BEEPER, LOW);

}



// Dodane

void Wheels::goForward(int cm) {
    cnt0 = 0;
    cnt1 = 0;
    int target = cm * TICKS_PER_CM;

    this->setSpeed(200);
    this->forward();

    this->lcd.clear();
    this->lcd.setCursor(0, 1);
    this->lcd.print(cm, DEC);
    this->displayAnimation();

    unsigned long lastUpdate = millis();
    while ((cnt0 + cnt1) / 2 < target) {
        if (millis() - lastUpdate >= 250) {
            lastUpdate = millis();
            int remainingCm = (target - (cnt0 + cnt1) / 2) / TICKS_PER_CM;
            this->lcd.setCursor(0, 1);
            this->lcd.print(remainingCm, DEC);
            this->lcd.print("cm  ");
            this->displayAnimation();
            Serial.println(remainingCm);
        }
    }

    this->lcd.clear();
    this->stop();
}

void Wheels::goBack(int cm) {
    cnt0 = 0;
    cnt1 = 0;
    int target = cm * TICKS_PER_CM;

    this->setSpeed(200);
    this->back();

    this->lcd.clear();
    this->lcd.setCursor(0, 1);
    this->lcd.print(cm, DEC);
    this->displayAnimation();

    unsigned long lastUpdate = millis();
    while ((cnt0 + cnt1) / 2 < target) {
        if (millis() - lastUpdate >= 250) {
            lastUpdate = millis();
            int remainingCm = (target - (cnt0 + cnt1) / 2) / TICKS_PER_CM;
            this->lcd.setCursor(0, 1);
            this->lcd.print(remainingCm, DEC);
            this->lcd.print("cm  ");
            this->displayAnimation();
            Serial.println(remainingCm);
        }
    }

    this->lcd.clear();
    this->stop();
}


void Wheels::turnLeft(int deg) {
    cnt0 = 0;
    cnt1 = 0;
    int target = (long)deg * TICKS_90DEG / 90;
    if (target < 1) target = 1;

    this->setSpeed(200);
    this->forwardRight();
    this->backLeft();

    while ((cnt0 + cnt1) / 2 < target) {}
    this->stop();
}

void Wheels::turnRight(int deg) {
    cnt0 = 0;
    cnt1 = 0;
    int target = (long)deg * TICKS_90DEG / 90;
    if (target < 1) target = 1;

    this->setSpeed(200);
    this->forwardLeft();
    this->backRight();

    while ((cnt0 + cnt1) / 2 < target) {}
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
    digitalWrite(BEEPER, digitalRead(BEEPER) ^ 1);
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


