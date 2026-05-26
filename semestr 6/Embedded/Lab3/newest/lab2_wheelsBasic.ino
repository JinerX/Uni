/*
 * Sterowanie autkiem: enkodery kół, sonar HC-SR04, serwo SG-90, pilot IR (NEC).
 *
 * TRYB SKANERA: odkomentuj poniższe #define, wgraj, otwórz Serial Monitor 9600.
 * Naciskaj klawisze pilota i zapisuj wypisywane kody hex.
 * Następnie wpisz je w sekcji "Kody IR" poniżej i zakomentuj SCAN_MODE.
 */
// #define SCAN_MODE

// Definiuj protokół PRZED dołączeniem biblioteki (oszczędza ~2kB Flash)
#define DECODE_NEC
#include <IRremote.hpp>

#include "Wheels.h"
#include "LiquidCrystal_I2C.h"

// ── Piny silników i głośnika ──────────────────────────────────────────────────
byte LCDAddress = 0x27;
LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

int EnA = 5,  EnB = 9;
int In1 = 6,  In2 = 7,  In3 = 10, In4 = 11;
int speaker_pin = 8;

// ── Piny peryferiów ───────────────────────────────────────────────────────────
#define TRIG_PIN    A4
#define ECHO_PIN    A5
#define SERVO_PIN   3
#define IR_PIN      2   // INT0 – wymagany przez IRRemote na ATmega328

// ── Kody IR (typowy pilot NEC 21-klawiszy z zestawów Arduino) ─────────────────
// Jeśli twój pilot jest inny – odkryj kody w trybie SCAN_MODE i wpisz tutaj.
#define KEY_0     0x16
#define KEY_1     0x45
#define KEY_2     0x46
#define KEY_3     0x47
#define KEY_4     0x44
#define KEY_5     0x40
#define KEY_6     0x43
#define KEY_7     0x07
#define KEY_8     0x15
#define KEY_9     0x09
#define KEY_STAR  0x42  // * – odblokowuje okno poleceń (anty-zakłócenia)
#define KEY_HASH  0x52  // # – stop / anuluj
#define KEY_UP    0x18  // naprzód
#define KEY_DOWN  0x4A  // wstecz
#define KEY_LEFT  0x08  // skręt w lewo
#define KEY_RIGHT 0x5A  // skręt w prawo
#define KEY_OK    0x1C  // OK – przełącz tryb MANUAL ↔ AUTO

// ── PIN dostępu (4 cyfry; zmień na swój) ─────────────────────────────────────
const uint8_t SECRET_PIN[]  = {1, 3, 3, 7};
#define PIN_LENGTH 4

// ── Parametry jazdy ───────────────────────────────────────────────────────────
#define OBSTACLE_CM   25   // zatrzymaj się gdy przeszkoda bliżej niż X cm
#define SESSION_MS   1500  // po naciśnięciu * okno akceptacji poleceń [ms]

// ── Stan aplikacji ────────────────────────────────────────────────────────────
enum Mode { MANUAL, AUTO };
Mode     driveMode    = MANUAL;
unsigned long sessionUnlock = 0; // timestamp ostatniego naciśnięcia *

Wheels w(lcd);

// ── Deklaracje pomocnicze ─────────────────────────────────────────────────────
int8_t  irCodeToDigit(uint8_t cmd);
void    enterPIN();
void    handleIR(uint8_t cmd, bool isRepeat);
void    avoidObstacle();

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.clear();

    w.attach(In4, In3, EnB, In2, In1, EnA, speaker_pin);
    w.attachSensors(A0, A1);
    w.attachSonar(TRIG_PIN, ECHO_PIN, SERVO_PIN);
    w.setSpeed(180);

    IrReceiver.begin(IR_PIN, DISABLE_LED_FEEDBACK);

#ifdef SCAN_MODE
    // ── Tryb skanera: wypisuje kody odebranych klawiszy ──────────────────────
    lcd.print("IR SCAN MODE");
    Serial.println("=== SCAN MODE – naciskaj klawisze pilota ===");
    while (true) {
        if (IrReceiver.decode()) {
            if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
                uint8_t cmd = IrReceiver.decodedIRData.command;
                Serial.print("cmd = 0x");
                Serial.println(cmd, HEX);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("cmd=0x");
                lcd.print(cmd, HEX);
            }
            IrReceiver.resume();
        }
    }
#else
    // ── Normalny start: wymagany PIN ─────────────────────────────────────────
    enterPIN();
#endif
}

// ─────────────────────────────────────────────────────────────────────────────
void loop() {
    // Sprawdź IR (dane buforowane sprzętowo – nie blokuje pętli)
    if (IrReceiver.decode()) {
        uint8_t cmd     = IrReceiver.decodedIRData.command;
        bool isRepeat   = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
        IrReceiver.resume();
        handleIR(cmd, isRepeat);
    }

    // Tryb autonomiczny (sonar)
    if (driveMode == AUTO) {
        w.forward();
        unsigned int dist = w.measureDistance(90);

        lcd.setCursor(0, 0);
        lcd.print("90:");
        lcd.print(dist);
        lcd.print("cm   ");
        Serial.println(dist); // dane dla Kreślarki

        if (dist > 0 && dist < OBSTACLE_CM) {
            w.stop();
            avoidObstacle();
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// handleIR – interpretuje kod IR z mechanizmem anty-zakłóceniowym.
//
// Mechanizm: klawisz * "odblokowuje sesję" na SESSION_MS ms.
// Polecenia ruchu są przyjmowane TYLKO w otwartym oknie sesji (lub jako
// auto-repeat trzymanego klawisza). Sporadyczne impulsy z obcych pilotów
// nie aktywują ruchu, bo nie poprzedza ich naciśnięcie *.
// ─────────────────────────────────────────────────────────────────────────────
void handleIR(uint8_t cmd, bool isRepeat) {
    // * – otwórz okno sesji (zawsze aktywny, nawet poza sesją)
    if (cmd == KEY_STAR && !isRepeat) {
        sessionUnlock = millis();
        lcd.setCursor(0, 0);
        lcd.print("* GOTOWY        ");
        Serial.println("Sesja otwarta");
        return;
    }

    bool sessionActive = (millis() - sessionUnlock) < SESSION_MS;

    // Polecenia ruchu wymagają aktywnej sesji (lub są auto-repeatem trzymanego klawisza)
    if (!sessionActive && !isRepeat) return;

    switch (cmd) {
        case KEY_UP:
            driveMode = MANUAL;
            w.setSpeed(180); w.forward();
            lcd.setCursor(0, 0); lcd.print("NAPRZOD         ");
            break;

        case KEY_DOWN:
            driveMode = MANUAL;
            w.setSpeed(180); w.back(); // back() aktywuje sygnał cofania
            lcd.setCursor(0, 0); lcd.print("WSTECZ          ");
            break;

        case KEY_LEFT:
            driveMode = MANUAL;
            w.setSpeed(150); w.forwardRight(); w.backLeft();
            lcd.setCursor(0, 0); lcd.print("LEWO            ");
            break;

        case KEY_RIGHT:
            driveMode = MANUAL;
            w.setSpeed(150); w.forwardLeft(); w.backRight();
            lcd.setCursor(0, 0); lcd.print("PRAWO           ");
            break;

        case KEY_OK:
            if (!isRepeat) {
                w.stop();
                driveMode = (driveMode == MANUAL) ? AUTO : MANUAL;
                lcd.setCursor(0, 0);
                lcd.print(driveMode == AUTO ? "TRYB: AUTO      "
                                            : "TRYB: MANUAL    ");
                Serial.println(driveMode == AUTO ? "AUTO" : "MANUAL");
            }
            break;

        case KEY_HASH:
            w.stop();
            driveMode = MANUAL;
            sessionUnlock = 0; // zamknij sesję po stop
            lcd.setCursor(0, 0); lcd.print("STOP            ");
            break;

        default:
            // Cyfry 1–9: regulacja prędkości (0 = minimalny ruch, 9 = maksymalny)
            if (!isRepeat) {
                int8_t digit = irCodeToDigit(cmd);
                if (digit > 0) {
                    uint8_t spd = (uint8_t)map(digit, 1, 9, 80, 255);
                    w.setSpeed(spd);
                    lcd.setCursor(0, 1);
                    lcd.print("spd="); lcd.print(spd); lcd.print("   ");
                }
            }
            break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// avoidObstacle – skanuje L/R i omija przeszkodę (tryb AUTO)
// ─────────────────────────────────────────────────────────────────────────────
void avoidObstacle() {
    int dir = w.scanAndDecide(); // -1=lewo, +1=prawo

    lcd.setCursor(0, 0);
    if (dir < 0) {
        lcd.print("OMIJAM: LEWO    ");
        w.turnLeft(90);
    } else {
        lcd.print("OMIJAM: PRAWO   ");
        w.turnRight(90);
    }

    w.goForward(30);

    if (dir < 0) w.turnRight(90); // wróć na kurs
    else         w.turnLeft(90);
}

// ─────────────────────────────────────────────────────────────────────────────
// enterPIN – blokuje setup() do chwili podania poprawnego kodu PIN
// ─────────────────────────────────────────────────────────────────────────────
void enterPIN() {
    while (true) {
        uint8_t buf[PIN_LENGTH];
        uint8_t pos = 0;

        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("Podaj PIN:");
        lcd.setCursor(0, 1);
        Serial.println("Podaj PIN (4 cyfry pilotem):");

        // Zbierz PIN_LENGTH cyfr
        while (pos < PIN_LENGTH) {
            if (IrReceiver.decode()) {
                bool isRepeat = IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;
                uint8_t cmd   = IrReceiver.decodedIRData.command;
                IrReceiver.resume();

                if (isRepeat) continue; // ignoruj auto-repeat

                int8_t digit = irCodeToDigit(cmd);
                if (digit >= 0) {
                    buf[pos++] = (uint8_t)digit;
                    lcd.print("*"); // zamiast cyfry (ukryty PIN)
                    Serial.print("*");
                }
            }
        }
        Serial.println();

        // Sprawdź zgodność
        bool ok = true;
        for (uint8_t i = 0; i < PIN_LENGTH; i++) {
            if (buf[i] != SECRET_PIN[i]) { ok = false; break; }
        }

        if (ok) {
            lcd.clear(); lcd.print("DOSTEP UDZIELONY");
            Serial.println("PIN OK – startujemy!");
            delay(1000);
            lcd.clear();
            return; // wyjdź z blokady
        } else {
            lcd.clear(); lcd.print("ZLY PIN!");
            Serial.println("Bledny PIN – sprobuj ponownie.");
            delay(1500);
            // pętla while powtarza wpisywanie
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
int8_t irCodeToDigit(uint8_t cmd) {
    switch (cmd) {
        case KEY_0: return 0;
        case KEY_1: return 1;
        case KEY_2: return 2;
        case KEY_3: return 3;
        case KEY_4: return 4;
        case KEY_5: return 5;
        case KEY_6: return 6;
        case KEY_7: return 7;
        case KEY_8: return 8;
        case KEY_9: return 9;
        default:    return -1;
    }
}


// ── Sterowanie manualne przez Serial (zachowane z poprzednich laboratoriów) ───
// Odkomentuj loop() poniżej i zakomentuj aktualny loop() by przywrócić.
//
// void loop() {
//   while(Serial.available()) {
//     char c = Serial.read();
//     switch(c) {
//       case 'w': w.forward(); break;
//       case 'x': w.back();    break;
//       case 'a': w.forwardRight(); w.backLeft();  break;
//       case 'd': w.forwardLeft();  w.backRight(); break;
//       case 's': w.stop();    break;
//       case '1': w.setSpeedLeft(75);  break;
//       case '2': w.setSpeedLeft(200); break;
//       case '9': w.setSpeedRight(75);  break;
//       case '0': w.setSpeedRight(200); break;
//       case '5': w.setSpeed(100); break;
//     }
//   }
// }
