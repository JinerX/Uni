#include <Arduino.h>
#include <TM1637Display.h>

#define CLK A5
#define DIO A4


class Ticker {
  private: 
    unsigned long period;
    unsigned long previousStart;
    void (*trigger)(void);
  public: 
    Ticker(unsigned long p, void(*fun)(void)) {
      period = p;
      trigger = fun;
      previousStart = 0;
    }
    void check() {
      unsigned long timeNow = millis();
      if(timeNow - previousStart > period) {
        // call function
        (*trigger)();
        previousStart = timeNow;
      }
    }
};

typedef enum { UP, DOWN } countDir_t;


TM1637Display display(CLK, DIO);

uint8_t cnt = 0; 
uint8_t k = 0;  // index variable to progress[]
uint8_t dispBrightness = 7;
countDir_t brightDir = DOWN;

const uint8_t pr0[] = { SEG_C | SEG_D | SEG_E };
const uint8_t pr1[] = { SEG_G |         SEG_D | SEG_E };
const uint8_t pr2[] = { SEG_G | SEG_C |         SEG_E };
const uint8_t pr3[] = { SEG_G | SEG_C | SEG_D };

const uint8_t* progress[] = {
  pr0, pr1, pr2, pr3
};


void smlT() {
    display.setSegments(progress[k], 1, 0);
    display.setSegments(progress[3-k], 1, 3);
    k = (k+1)%4;
}

void bigT() {
    display.showNumberDec(cnt, false, 2, 1);
    cnt = (cnt+1) % 100;
}

void dimT() {
  display.setBrightness(dispBrightness);
  if(brightDir == UP){
    dispBrightness = dispBrightness+1;
    if(dispBrightness == 5) {
      brightDir = DOWN;
    }
  } else if(brightDir == DOWN) {
    dispBrightness = dispBrightness-1;
    if(dispBrightness == 0) {
      brightDir = UP;
    }
  }
  Serial.println(dispBrightness);
}


Ticker aa(75, smlT);
Ticker bb(1000, bigT);
Ticker cc(150, dimT);


void setup() {

  Serial.begin(9600);

  display.setBrightness(0x0e); 
  display.clear();
}

void loop() {

  aa.check();
  bb.check();
  cc.check();
}
