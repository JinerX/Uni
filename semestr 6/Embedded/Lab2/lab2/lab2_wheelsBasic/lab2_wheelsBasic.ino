#include "Wheels.h"

int EnA = 5;
int EnB = 9;
int In1 = 6;
int In2 = 7;
int In3 = 10;
int In4 = 11;

Wheels w;
volatile char cmd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  w.attach(In4,In3,EnB,In2,In1,EnA);
  w.goForward(100);
  // delay(10000);
  // w.goBack(10);

  // Serial.println("Forward: WAD");
  // Serial.println("Back: ZXC");
  // Serial.println("Stop: S");

}
void loop() {

}

// void loop() {
//   while(Serial.available())
//   {
//     cmd = Serial.read();
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
