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


int EnA = 5;
int EnB = 9;
int In1 = 6;
int In2 = 7;
int In3 = 10;
int In4 = 11;

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
  // w.startAnimation(lcd);
  w.goForward(300);
  delay(5000);
  w.goBack(300);

}
void loop() {
  Serial.print("Time: ");
  myTime = millis();
  Serial.println(myTime);
  delay(1000);
}



// code for manual control


// int counter = 0;
// int display = 0;
// void loop() {
//   while(Serial.available())
//   {
//     cmd = Serial.read();
//     display = display - 1;
//     // Wyświetl tekst komendy na LCD
//     // lcd.clear();
//     // lcd.setCursor(0,1);
//     // lcd.print(display, DEC);

//     switch(cmd)
//     {
//       case 'w': w.forward(); break;
//       case 'x': w.back(); break;
//       case 'a': w.forwardLeft(); break;
//       case 'd': w.forwardRight(); break;
//       case 'z': w.backLeft(); break;
//       case 'c': w.backRight(); break;
//       case 's': w.stop(); break;
//       case '1': w.setSpeedLeft(75); break;
//       case '2': w.setSpeedLeft(200); break;
//       case '9': w.setSpeedRight(75); break;
//       case '0': w.setSpeedRight(200); break;
//       case '5': w.setSpeed(100); break;
//     }
//   }
// }
