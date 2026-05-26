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
#include "LiquidCrystal_I2C.h"

#ifndef Wheels_h
#define Wheels_h

#define BEEPER 13

#define TICKS_PER_CM  1
#define TICKS_90DEG   57



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
        void goForward(int cm);
        void goBack(int cm);
        void turnLeft(int deg);
        void turnRight(int deg);
        void setupSensors();
        static void makeSound();
        void TimerUpdate();

        static volatile int cnt0, cnt1;
        
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
        static Wheels* instance;
        bool goingBack = false;

};



#endif
