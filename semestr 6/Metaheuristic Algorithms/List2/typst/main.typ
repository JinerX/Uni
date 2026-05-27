#set document(title: "Algorytmy Metaheurystyczne — Lista 2")
#set page(paper: "a4", margin: (x: 2.5cm, y: 2.5cm))
#set text(font: "New Computer Modern", size: 11pt, lang: "pl")
#set heading(numbering: "1.1.")
#set par(justify: true, leading: 0.65em)

#let todo = text.with(fill: red)

// ── Title ──────────────────────────────────────────────────────────────────

#align(center)[
  #v(0.5em)
  #text(18pt, weight: "bold")[Algorytmy Metaheurystyczne — Lista 2]
  #v(0.5em)
  #text(12pt)[Symulowane Wyżarzanie oraz Przeszukiwanie z Zakazami \ dla Problemu Komiwojażera]
  #v(0.5em)
  Maj 2026, Jędrzej Sajnóg, indeks: 279701
  #v(0.3em)
  #line(length: 100%)
]

#v(0.5em)

// ══════════════════════════════════════════════════════════════════════════
= Zadanie 1 — Symulowane Wyżarzanie (SA)
// ══════════════════════════════════════════════════════════════════════════

== Opis problemu

Problem komiwojażera (TSP — *Travelling Salesman Problem*) polega na znalezieniu
najkrótszego cyklu Hamiltonowskiego w grafie pełnym: mając dane $n$ miast
o współrzędnych $(x_i, y_i)$, szukamy permutacji minimalizującej długość ścieżki.

W eksperymentach użyto instancji:
wi29 (29 miast), dj38 (38), qa194 (194), uy734 (734), zi929 (929) do
strojenia parametrów, a mu1979 (1979), ca4663 (4663), eg7146 (7146),
ei8246 (8246) i tz6117 (6117) do testów.

== Algorytm

Symulowane wyżarzanie:
Na każdym kroku losowany jest ruch sąsiedzki (tu: *2-opt* — odwrócenie
podsegmentu trasy). Ruch jest akceptowany zawsze, gdy poprawia rozwiązanie
($Delta < 0$), a z prawdopodobieństwem $e^{-Delta/T}$, gdy je pogarsza.
Temperatura $T$ jest stopniowo obniżana wg harmonogramu geometrycznego:

$
  T_{k+1} = alpha dot T_k, quad alpha in (0,1).
$

Schemat zatrzymania: SA działa od $T_0$ do $T approx 1$, co daje
$ceil(log(1\/T_0) \/ log(alpha))$ epok.  W każdej epoce wykonywanych jest
`iters_per_temp` prób ruchu.

Ruch 2-opt wybiera losowo dwa indeksy $i < j$ i odwraca segment trasy
$[i+1, j]$.  Zmiana kosztu obliczana jest bez przeliczania całej trasy:

$
  Delta = d(c_{pi(i)}, c_{pi(j)}) + d(c_{pi(i+1)}, c_{pi(j+1)})
        - d(c_{pi(i)}, c_{pi(i+1)}) - d(c_{pi(j)}, c_{pi(j+1)}).
$

== Implementacja

Implementacja w C++ (`tsp_sa.h`, `main.cpp`, `main2.cpp`):

- `run_sa(cities, params, rng)` — pojedyncze uruchomienie SA; zwraca
  najlepszą trasę i koszt znaleziony w trakcie całego biegu.
- Parametry: `init_temp` $T_0$, `cooling_rate` $alpha$, `iters_per_temp`.
- Inicjalizacja: losowa permutacja miast (shuffle z MT19937).
- Śledzenie najlepszego: zachowywana jest osobna kopia `best` aktualizowana
  przy każdym polepszeniu, niezależnie od bieżącej temperatury.

== Dobór parametrów (grid search)

Grid search wykonano na instancjach wi29–zi929 (jeden przebieg SA na
kombinację).  Przeszukiwana siatka:

#align(center, table(
  columns: (auto, auto),
  stroke: 0.5pt,
  inset: 6pt,
  align: left,
  [*Parametr*], [*Wartości*],
  [$T_0$],       [1000, 5000, 6000, 10 000],
  [$alpha$],     [0.85, 0.9, 0.985, 0.990],
  [`iters_per_temp`], [1000, 2000, 5000, 7000, 10 000],
))

Chociaż nie naraz (za dużo możliwych kombinacji). Zamiast tego testowano mniejszą liczbę kombinacji i na podstawie najlepszej decydowano się na zmianę przeszukiwanych wartości.


Kryterium oceny — suma względnych kosztów:

$
  "score" = sum_(i=1)^{N_"inst"} frac(c_i, c_i^*),
$

gdzie $c_i^*$ to najlepszy koszt uzyskany przez *jakąkolwiek* kombinację na
instancji $i$.  Niższa wartość = lepiej.

Najlepsze parametry: $T_0 = 10000$, $alpha = 0.99$, `iters_per_temp` $= 10000$.

Wyniki dla poszczególnych instancji z najlepszymi parametrami:

#align(center, table(
  columns: (auto, auto, auto),
  stroke: 0.5pt,
  inset: 6pt,
  align: center,
  table.header([Instancja], [$n$], [Koszt SA]),
  [wi29],  [29],  [27 601],
  [dj38],  [38],  [6 659],
  [qa194], [194], [9 540],
  [uy734], [734], [85 297],
  [zi929], [929], [104 339],
))

== Wykresy

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad1/results/solution_wi29.png",  width: 100%),
    image("../zad1/results/solution_dj38.png",  width: 100%),
  ),
  caption: [Trasy SA dla wi29 i dj38.],
)

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad1/results/solution_uy734.png", width: 100%),
    image("../zad1/results/solution_zi929.png", width: 100%),
  ),
  caption: [Trasy SA dla uy734 i zi929.]
)

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad1/results/mu1979_best_tour.png", width: 100%),
    image("../zad1/results/ca4663_best_tour.png", width: 100%),
  ),
  caption: [Trasy SA dla mu1979 i ca4663.],
)

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad1/results/tz6117_best_tour.png", width: 100%),
    image("../zad1/results/eg7146_best_tour.png", width: 100%),
  ),
  caption: [Trasy SA dla tz6117 i eg7146.],
)

#figure(
  grid(
    columns: 1,
    gutter: 0.5em,
    image("../zad1/results/ei8246_best_tour.png", width: 100%),
  ),
  caption: [Trasa SA dla ei8246.],
)



== Obserwacje i wnioski

- *Rola $alpha$ i `iters_per_temp`:* Wolniejsze chłodzenie ($alpha=0.990$)
  oraz więcej iteracji na temperaturę konsekwentnie daje lepsze wyniki.
  Wyższe $T_0$ lekko przeważa nad niższym przy tych samych pozostałych
  parametrach, możliwie ze względu na większą liczbę iteracji.

- *Porównanie z poprzednimi listami:* Wyniki są około 50% gorsze w porównaniu z poprzednimi metodami, szczególnie dla dużej liczby miast.
- *Rozwiązania:* SA dobrze znajduje "główny wygląd" ścieżki, ale ma problemy ze znalezieniem dokładniejszego rozwiązania.

// ══════════════════════════════════════════════════════════════════════════
= Zadanie 2 — Przeszukiwanie z Zakazami (TS)
// ══════════════════════════════════════════════════════════════════════════

== Algorytm

Przeszukiwanie z zakazami (*Tabu Search*) to algorytm
lokalnego przeszukiwania z pamięcią krótkoterminową.  W każdej iteracji
wybierany jest *najlepszy* dopuszczalny ruch spośród próbki sąsiedztwa —
nawet jeśli pogarsza on bieżące rozwiązanie.  Zapobiega to zapętleniom
dzięki *liście tabu*: ostatnio wykonane ruchy są tymczasowo zakazane.

*Kryterium aspiracji:* ruch tabu jest dopuszczalny, jeśli prowadzi do nowego
globalnego minimum (przebicie bieżącego tabu).

Warunek stopu: brak poprawy globalnego minimum przez `max_no_improve`
kolejnych iteracji.

== Implementacja

Implementacja w (`tsp_ts.h`, `main.cpp`, `main2.cpp`):

- Lista tabu przechowuje pary $(a, b)$ z $a < b$ — wewnętrzne końcowe
  indeksy odwróconego segmentu.
- W każdej iteracji losowane jest `neighbor_sample` kandydatów 2-opt;
  wybierany jest ten o najniższym $Delta$ wśród dozwolonych lub
  spełniających kryterium aspiracji.
- Parametry: `tabu_size`, `neighbor_sample`, `max_no_improve`.
- Inicjalizacja: ta sama co w SA — losowa permutacja.

== Dobór parametrów (grid search)

#align(center, table(
  columns: (auto, auto),
  stroke: 0.5pt,
  inset: 6pt,
  align: left,
  [*Parametr*], [*Wartości*],
  [`tabu_size`],       [10, 50, 100],
  [`neighbor_sample`], [50, 200, 500],
  [`max_no_improve`],  [300, 1000, 3000],
))

Ta sama metryka score co w SA.



Najlepsze parametry: `tabu_size`$=10$, `neighbor_sample`$=500$,
`max_no_improve`$=3000$.

Wyniki dla poszczególnych instancji:

#align(center, table(
  columns: (auto, auto, auto),
  stroke: 0.5pt,
  inset: 6pt,
  align: center,
  table.header([Instancja], [$n$], [Koszt TS]),
  [wi29],  [29],  [27 601],
  [dj38],  [38],  [6 659],
  [qa194], [194], [9 875],
  [uy734], [734], [93 513],
  [zi929], [929], [104 514],
))

== Wykresy

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad2/results/solution_wi29.png",  width: 100%),
    image("../zad2/results/solution_dj38.png",  width: 100%),
  ),
  caption: [Trasy TS dla wi29 i dj38 — najlepsze parametry.],
)

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad2/results/solution_uy734.png", width: 100%),
    image("../zad2/results/solution_zi929.png", width: 100%),
  ),
  caption: [Trasy TS dla uy734 (734 miast) i zi929 (929 miast).],
)

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad2/results/mu1979_best_tour.png", width: 100%),
    image("../zad2/results/ca4663_best_tour.png", width: 100%),
  ),
  caption: [Trasy SA dla mu1979 i ca4663.],
)

#figure(
  grid(
    columns: 2,
    gutter: 0.5em,
    image("../zad2/results/tz6117_best_tour.png", width: 100%),
    image("../zad2/results/eg7146_best_tour.png", width: 100%),
  ),
  caption: [Trasy SA dla tz6117 i eg7146.],
)

#figure(
  grid(
    columns: 1,
    gutter: 0.5em,
    image("../zad2/results/ei8246_best_tour.png", width: 100%),
  ),
  caption: [Trasa SA dla ei8246.],
)



== Obserwacje i wnioski

- *Kluczowy wpływ `neighbor_sample`:* Wyniki z `sample`$=50$ są dramatycznie
  gorsze niż z `sample`$=500$.  Ocena zbyt małej próbki
  sąsiedztwa prowadzi do wyborów praktycznie losowych.

- *Małe `tabu_size` najlepsze:* `tabu_size`$=10$ wygrywa we wszystkich
  konfiguracjach z `sample`$=500$.  Zbyt duża lista tabu nadmiernie
  ogranicza przestrzeń dopuszczalnych ruchów.

- *Zmienność wyników:* Wyniki mają dosyć wysoką rozpiętość (zdjęcia osobno).


// ══════════════════════════════════════════════════════════════════════════
= Porównanie algorytmów
// ══════════════════════════════════════════════════════════════════════════





== Wnioski końcowe

*SA osiąga lepsze wyniki jakościowe* TS daje lepsze wyniki od SA szczególnie na dużych instancjach problemu. Na mniejszych instancjach osiągają porównywalne wyniki.

*Porównanie z poprzednimi listami* TS daje wyniki porównywalne z poprzednimi podejściami np. z MST. Chociaż zwykle daje wyniki lekko gorsze. SA daje wyniki znacząco gorsze.


*Wrażliwość na parametry:* SA jest stosunkowo mało wrażliwy — różnice
między najlepszą a najgorszą kombinacją wynoszą na małych instancjach.
TS jest bardzo wrażliwy na `neighbor_sample` — zła wartość (50) jest gorsza
o >40% od optymalnej (500).

*Ograniczenia SA:* przy dużych instancjach ($n > 4000$) zadana liczba iteracji nie wystarcza, wyniki dla ca4663 i większych.
