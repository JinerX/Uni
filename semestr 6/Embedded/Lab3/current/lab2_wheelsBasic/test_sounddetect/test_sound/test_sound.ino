const int soundPin = 2;

void setup() {
    pinMode(soundPin, INPUT);
    Serial.begin(9600);
}

void loop() {

    if(digitalRead(soundPin) == LOW) {
        Serial.println("Dzwiek!");
    }

    delay(100);
}