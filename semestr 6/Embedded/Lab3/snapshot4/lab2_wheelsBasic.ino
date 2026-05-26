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

// Sonar HC-SR04 i serwo SG-90
#define TRIG_PIN   A4
#define ECHO_PIN   A5
#define SERVO_PIN  3
#define OBSTACLE_CM 25  // zatrzymaj się gdy przeszkoda bliżej niż X cm

Wheels w(lcd);
volatile char cmd;

unsigned long myTime;

void avoidObstacle();


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
  w.attachSensors(A0, A1);            // enkodery: prawe=A0, lewe=A1
  w.attachSonar(TRIG_PIN, ECHO_PIN, SERVO_PIN); // sonar + serwo
  w.setSpeed(180);
}

void loop() {
  w.forward();

  unsigned int dist = w.measureDistance(90); // patrz prosto przed siebie

  // wyświetl kąt i odległość na LCD (wiersz 0) i Serial (Kreślarka)
  lcd.setCursor(0, 0);
  lcd.print("90: ");
  lcd.print(dist);
  lcd.print("cm   ");
  Serial.println(dist);

  if (dist > 0 && dist < OBSTACLE_CM) {
    w.stop();
    avoidObstacle();
  }
}

// Skanuje lewo/prawo, wybiera stronę z większą przestrzenią i omija przeszkodę.
void avoidObstacle() {
  int dir = w.scanAndDecide(); // -1=lewo, +1=prawo

  lcd.setCursor(0, 0);
  if (dir < 0) {
    lcd.print("OMIJAM: LEWO  ");
    w.turnLeft(90);
  } else {
    lcd.print("OMIJAM: PRAWO ");
    w.turnRight(90);
  }

  w.goForward(30); // przejedź obok przeszkody

  // wróć na pierwotny kierunek (odwrotny skręt)
  if (dir < 0) w.turnRight(90);
  else         w.turnLeft(90);
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
