#define LED 13

int unit = 200;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) {
    char c = Serial.read();

    if (c >= 'A' && c <= 'Z') c += 32;

    Serial.print("Litera: ");
    Serial.println(c);

    sendMorse(c);
  }
}

void dot() {
  digitalWrite(LED, HIGH);
  delay(unit);
  digitalWrite(LED, LOW);
  delay(unit);
}

void dash() {
  digitalWrite(LED, HIGH);
  delay(3 * unit);
  digitalWrite(LED, LOW);
  delay(unit);
}

void sendMorse(char c) {

  switch(c) {

    case 'a': dot(); dash(); break;
    case 'b': dash(); dot(); dot(); dot(); break;
    case 'c': dash(); dot(); dash(); dot(); break;
    case 'd': dash(); dot(); dot(); break;
    case 'e': dot(); break;
    case 'f': dot(); dot(); dash(); dot(); break;
    case 'g': dash(); dash(); dot(); break;
    case 'h': dot(); dot(); dot(); dot(); break;
    case 'i': dot(); dot(); break;
    case 'j': dot(); dash(); dash(); dash(); break;
    case 'k': dash(); dot(); dash(); break;
    case 'l': dot(); dash(); dot(); dot(); break;
    case 'm': dash(); dash(); break;
    case 'n': dash(); dot(); break;
    case 'o': dash(); dash(); dash(); break;
    case 'p': dot(); dash(); dash(); dot(); break;
    case 'q': dash(); dash(); dot(); dash(); break;
    case 'r': dot(); dash(); dot(); break;
    case 's': dot(); dot(); dot(); break;
    case 't': dash(); break;
    case 'u': dot(); dot(); dash(); break;
    case 'v': dot(); dot(); dot(); dash(); break;
    case 'w': dot(); dash(); dash(); break;
    case 'x': dash(); dot(); dot(); dash(); break;
    case 'y': dash(); dot(); dash(); dash(); break;
    case 'z': dash(); dash(); dot(); dot(); break;

    default:
      Serial.println("Nieznany znak");
  }

  delay(3 * unit);
}