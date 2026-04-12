#include "Ticker.h"
#include <Arduino.j>

Ticker::Ticker(unsigned long p, void(*fun)(void)) {
    period = p;
    trigger = fun;
    previousStart = 0;
}

Ticker::void check() {
    unsigned long timeNow = millis();
    if(timeNow - previousStart > period) {
    // call function
    (*trigger)();
    previousStart = timeNow;
    }
}