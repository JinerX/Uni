/* 
 * prosta implementacja klasy obsługującej 
 * silniki pojazdu za pośrednictwem modułu L298
 *
 * Sterowanie odbywa się przez:
 * 1)  powiązanie odpowiednich pinów I/O Arduino metodą attach() 
 * 2)  ustalenie prędkości setSpeed*()
 * 3)  wywołanie funkcji ruchu
 *
 * TODO:
 *  - zabezpieczenie przed ruchem bez attach()
 *  - ustawienie domyślnej prędkości != 0
 */


#include <Arduino.h>
#include <Servo.h>
#include "LiquidCrystal_I2C.h"

#ifndef Wheels_h
#define Wheels_h



class Wheels {
    public: 
        Wheels(LiquidCrystal_I2C& lcd);
        /*
         *  pinForward - wejście "naprzód" L298
         *  pinBack    - wejście "wstecz" L298
         *  pinSpeed   - wejście "enable/PWM" L298
         */
        void attachRight(int pinForward, int pinBack, int pinSpeed);
        void attachLeft(int pinForward, int pinBack, int pinSpeed);
        void attach(int pinRightForward, int pinRightBack, int pinRightSpeed,
                    int pinLeftForward, int pinLeftBack, int pinLeftSpeed, int speaker_pin);
        /*
         *  funkcje ruchu
         */
        void forward();
        void forwardLeft();
        void forwardRight();
        void back();
        void backLeft();
        void backRight();
        void stop();
        void stopLeft();
        void stopRight();
        void displayAnimation();

        /***
         *
         * Dodane
         *
         */
        void attachSensors(int sensorRight, int sensorLeft);
        void attachSonar(int trig, int echo, int servoPin);
        void goForward(int cm);
        void goBack(int cm);
        void turnLeft(int degrees);
        void turnRight(int degrees);
        unsigned int measureDistance(byte angle); // zwraca odległość w cm
        int scanAndDecide();                      // -1=lewo, +1=prawo
        static void makeSound();
        void TimerUpdate();

        // Liczniki enkoderów (volatile – dostęp z ISR)
        static volatile uint16_t cnt0; // prawe koło (A0/PC0)
        static volatile uint16_t cnt1; // lewe koło  (A1/PC1)
        // Stałe kalibracyjne – dostosuj do swojego pojazdu
        static const uint8_t TICKS_PER_CM  = 4;  // ticki na centymetr
        static const uint8_t WHEEL_TRACK_CM = 14; // rozstaw kół [cm]
        
        /*
         *  ustawienie prędkości obrotowej (przez PWM)
         *   - minimalna efektywna wartość 60
         *      może zależeć od stanu naładowania baterii
         */
        void setSpeed(uint8_t);
        void setSpeedRight(uint8_t);
        void setSpeedLeft(uint8_t);

        int speed_right;
        int speed_left;
        LiquidCrystal_I2C& lcd;

    private:
        int pinsRight[3];
        int pinsLeft[3];
        int speaker_pin;
        int trigPin;
        int echoPin;
        Servo serwo;
        static Wheels* instance;

        volatile bool buzzerState = false;
        volatile uint16_t tickCount = 0;
        uint16_t beepOnTicks = 3;
        uint16_t beepOffTicks = 20;
};



#endif
