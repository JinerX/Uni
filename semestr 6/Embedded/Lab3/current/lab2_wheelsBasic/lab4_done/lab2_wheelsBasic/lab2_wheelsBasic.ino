#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

uint8_t arrowRight[8] =
{
    0b01000,
    0b01100,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b01100,
    0b01000
};

// int argIn = 0;


int EnA = 6;
int EnB = 5;
int In1 = 8;
int In2 = 2;
int In3 = 4;
int In4 = 12;

int speaker_pin = 8;


Wheels w(lcd);
volatile char cmd;

unsigned long myTime;


// example move forward move backward in cm

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(200);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // lcd.createChar(0, "2");
  w.attach(In4,In3,EnB,In2,In1,EnA, speaker_pin);
  w.setupSensors();
  w.goForward(50);
  delay(500);
  w.turnLeft(90);
  delay(500);
  w.goForward(50);
  delay(500);
  w.turnRight(90);
  delay(500);
  w.goForward(50);

}
// void loop() {
  // Serial.print("Time: ");
  // myTime = millis();
  // Serial.println(myTime);
  // delay(1000);
// }

ISR(PCINT1_vect) {
  if (PINC & (1 << PC0)) Wheels::cnt0++;
  if (PINC & (1 << PC1)) Wheels::cnt1++;
}



// code for manual control


int counter = 0;
int display = 0;
void loop() {
  while(Serial.available())
  {
    Serial.println(w.speed_right);
    cmd = Serial.read();
    display = display - 1;
    // Wyświetl tekst komendy na LCD
    // lcd.clear();
    // lcd.setCursor(0,1);
    // lcd.print(display, DEC);

    switch(cmd)
    {
      case 'w': w.forward(); break;
      case 'x': w.back(); break;
      case 's': w.stop(); break;
      case '1': w.setSpeed(50); break;
      case '2': w.setSpeed(100); break;
      case '3': w.setSpeed(150); break;
      case '4': w.setSpeed(200); break;
      case '5': w.setSpeed(250); break;
    }
  }
}
