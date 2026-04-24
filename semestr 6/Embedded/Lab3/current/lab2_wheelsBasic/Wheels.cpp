#include <TimerOne.h>

#include <Arduino.h>

#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)


Wheels* Wheels::instance = nullptr;

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
    if (speed < 1) {
        speed = 1;
    }
    unsigned long period = map(speed, 60, 255, 400000UL, 1000000UL);
    Timer1.detachInterrupt();
    Timer1.attachInterrupt(Wheels::makeSound, 1000);
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
    unsigned long start_time = millis();
    unsigned long curr_time = start_time;

    this->setSpeed(200);
    this->forward();

    this->lcd.clear();
    this->lcd.setCursor(0,1);
    this->lcd.print(cm, DEC);
    this->displayAnimation();


    int mult = 23;
    int sectors = 10;

    unsigned long total_time = mult * cm;
    unsigned long interval = total_time / sectors;
    unsigned long last_update = start_time;

    while (curr_time < start_time + total_time) {
        curr_time = millis();

        // update only every interval
        if (curr_time - last_update >= interval) {
            last_update = curr_time;

            unsigned long elapsed = curr_time - start_time;
            int remaining = cm - (cm * elapsed) / total_time;

            this->lcd.clear();
            this->lcd.setCursor(0,1);
            this->lcd.print(remaining, DEC);
            this->displayAnimation();
        }
    }
    this->lcd.setCursor(0,1);
    this->lcd.print(0, DEC);
    this->lcd.clear();
    this->stop();
}

void Wheels::goBack(int cm) {
    unsigned long start_time = millis();
    unsigned long curr_time = start_time;

    this->setSpeed(200);
    this->back();

    this->lcd.clear();
    this->lcd.setCursor(0,1);
    this->lcd.print(cm, DEC);
    this->displayAnimation();

    int mult = 23;
    int sectors = 10;

    unsigned long total_time = mult * cm;
    unsigned long interval = total_time / sectors;
    unsigned long last_update = start_time;

    while (curr_time < start_time + total_time) {
        curr_time = millis();

        // update only every interval
        if (curr_time - last_update >= interval) {
            last_update = curr_time;

            unsigned long elapsed = curr_time - start_time;
            int remaining = cm - (cm * elapsed) / total_time;

            this->lcd.clear();
            this->displayAnimation();
            this->lcd.setCursor(0,1);
            this->lcd.print(remaining, DEC);
        }
    }
    this->lcd.setCursor(0,1);
    this->lcd.print(0, DEC);
    this->lcd.clear();
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
    instance->buzzerState = !instance->buzzerState;
    digitalWrite(instance->speaker_pin, instance->buzzerState ? HIGH : LOW);
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


