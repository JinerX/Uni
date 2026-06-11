# Czujnik dźwięku LM393 do Arduino — notatka praktyczna

## Co to jest
Moduł „sound sensor” z LM393 to nie jest sam mikrofon, tylko mały układ z mikrofonem, komparatorem LM393 i zwykle potencjometrem do ustawiania czułości. W praktyce dostajesz dwa sygnały:

- **AO / A0** — sygnał analogowy, czyli „jak mocny” jest dźwięk.
- **DO / D0** — sygnał cyfrowy, czyli czy dźwięk przekroczył ustawiony próg.

To sprawia, że moduł jest prosty w użyciu do wykrywania głośnych dźwięków, stuknięć i klaskania.

## Jak to działa
Mikrofon zamienia falę dźwiękową na sygnał elektryczny. Ten sygnał trafia do komparatora LM393. Komparator porównuje go z progiem ustawionym potencjometrem. Gdy dźwięk jest wystarczająco głośny, wyjście cyfrowe zmienia stan.

W praktyce:

- jeśli chcesz tylko wiedzieć „czy coś głośnego się wydarzyło”, używaj **DO**,
- jeśli chcesz lepiej stroić próg, wykrywać pojedynczy clap albo liczyć klaski, obserwuj **AO** i ustaw własny próg w kodzie.

## Podłączenie do Arduino UNO
Typowy moduł ma piny opisane jako:

- **VCC / +** → 5V lub 3.3V zależnie od wersji modułu
- **GND** → GND
- **DO / D0** → dowolny pin cyfrowy, np. D2
- **AO / A0** → dowolny pin analogowy, np. A0

Najprostszy układ do testów:

- VCC → 5V
- GND → GND
- DO → D2
- AO → A0

## Ustawianie czułości
Na płytce jest mały potencjometr.

1. Wgraj prosty program i otwórz Serial Monitor.
2. Kręć potencjometrem bardzo powoli.
3. Zrób pojedyncze klaśnięcie blisko mikrofonu.
4. Ustaw tak, żeby zwykły hałas nie wyzwalał czujnika, a klaśnięcie już tak.

Dobrze jest testować w warunkach, w których czujnik ma pracować. Inaczej w pustym pokoju ustawisz próg za nisko, a w głośnym otoczeniu za wysoko.

## Najprostszy test działania
Najpierw sprawdź sam sygnał z DO.

```cpp
const int soundPin = 2;
const int ledPin = 13;

void setup() {
  pinMode(soundPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int state = digitalRead(soundPin);
  digitalWrite(ledPin, state);
  Serial.println(state);
  delay(10);
}
```

Jeśli po klaśnięciu dioda się zapala albo w Serial Monitorze pojawia się zmiana stanu, moduł działa.

## Wykrywanie klaskania w praktyce
Do klaskania najlepiej nie reagować na sam stan „ciągle 1”, tylko na **krótki impuls**. Klaskanie jest właśnie takim impulsem: nagły, krótki wzrost głośności.

Najprostsza wersja to:

- wykryj zbocze narastające albo moment, w którym czujnik zmienia stan,
- dodaj krótkie opóźnienie antydrganiowe,
- jeśli chcesz sterować przełącznikiem, zmieniaj stan LED po każdym poprawnym klaśnięciu.

### Przykład: przełączanie LED jednym klaśnięciem

```cpp
const int soundPin = 2;
const int ledPin = 13;

bool ledState = false;
int lastState = LOW;
unsigned long lastClapTime = 0;
const unsigned long debounceMs = 200;

void setup() {
  pinMode(soundPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int currentState = digitalRead(soundPin);

  if (currentState == HIGH && lastState == LOW) {
    unsigned long now = millis();
    if (now - lastClapTime > debounceMs) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      lastClapTime = now;
      Serial.println("Clap detected");
    }
  }

  lastState = currentState;
}
```

Jeżeli Twoja płytka działa odwrotnie i impuls pojawia się jako LOW, odwróć warunek w `if`.

## Wykrywanie podwójnego klasknięcia
To jest bardzo praktyczne w automatyce. Jedno klaśnięcie nie robi nic, dwa szybkie klaski włączają światło.

Zasada:

- pierwszy clap zapisuje czas,
- drugi clap musi nadejść w krótkim oknie, np. 300–600 ms,
- jeśli tak się stanie, wykonujesz akcję.

To zwykle działa lepiej niż reagowanie na pojedynczy impuls, bo przypadkowy hałas rzadziej daje dwa podobne sygnały pod rząd.

## A0 czy DO?

**DO** używaj, gdy chcesz szybko zbudować prosty „clap switch”.

**AO** używaj, gdy chcesz:

- samemu ustawić próg w kodzie,
- filtrować krótkie zakłócenia,
- porównać poziom tła z nagłym impulsem,
- lepiej stroić wykrywanie w zależności od pokoju.

## Praktyczne wskazówki

- Trzymaj mikrofon dość blisko miejsca klaskania.
- Unikaj ustawiania czułości na maksimum, bo czujnik będzie łapał każdy szum.
- Testuj w docelowym otoczeniu, bo próg w ciszy i w normalnym pokoju może być zupełnie inny.
- Jeśli czujnik reaguje na rozmowę albo trzask drzwi, zwiększ próg albo dodaj logikę opóźnienia.
- Dla stabilniejszego sterowania lepszy bywa **double-clap** niż pojedynczy clap.

## Najważniejsze wnioski

- LM393 w takim module działa jak prosty komparator progu głośności.
- DO daje najłatwiejsze wykrywanie klaskania.
- AO przydaje się do własnych algorytmów i strojenia czułości.
- Do praktycznych projektów najlepiej użyć krótkiego impulsu, debouncu i ewentualnie podwójnego klaskania.
