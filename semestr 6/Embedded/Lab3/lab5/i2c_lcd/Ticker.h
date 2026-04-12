#ifndef TICKER_H
#define TICKER_H

#include <Arduino.h>


class Ticker {
  private:
    unsigned long period;
    unsigned long previousStart;
    void (*trigger)(void);
  public:
    Ticker(unsigned long p, void(*fun)(void));
    void check();
};

#endif