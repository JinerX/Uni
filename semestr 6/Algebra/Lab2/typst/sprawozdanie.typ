// ============================================================
//  Sprawozdanie 2 — Lab. 4
//  Wybrane Zagadnienia Algebry 2026
// ============================================================

#set document(title: "Sprawozdanie 2 — Lab. 4", author: "")
#set page(paper: "a4", margin: (x: 2.4cm, y: 2.2cm))
#set text(size: 10.5pt, lang: "pl")
#set par(justify: true, leading: 0.65em)
#set heading(numbering: none)

#show raw.where(block: true): r => block(
  fill: luma(250),
  stroke: (left: 3pt + blue.lighten(35%)),
  inset: (left: 12pt, right: 10pt, top: 7pt, bottom: 7pt),
  width: 100%,
  text(size: 8.5pt, r),
)

#show heading.where(level: 1): it => {
  v(1em)
  block(
    stroke: (bottom: 1pt + luma(180)),
    inset: (bottom: 3pt),
    width: 100%,
    text(size: 12.5pt, weight: "bold", it.body),
  )
  v(0.5em)
}

// ── NAGŁÓWEK ──────────────────────────────────────────────
#align(center)[
  #v(3mm)
  #text(size: 18pt, weight: "bold")[Sprawozdanie 2]
  #v(1mm)
  #text(size: 11.5pt, fill: luma(80))[Wybrane Zagadnienia Algebry 2026 · Lab. 4]
  #v(6mm)
  #grid(
    columns: (4.5cm, auto),
    row-gutter: 3mm,
    align: (right, left),
    text(weight: "bold")[Imię i nazwisko:], [IMIĘ NAZWISKO],
    text(weight: "bold")[Nr indeksu:],      [XXXXXX],
  )
  #v(5mm)
  #line(length: 100%, stroke: 0.5pt + luma(160))
]

#v(1em)

// ──────────────────────────────────────────────────────────
= Lab. 4a — Wielomian wielu zmiennych
// ──────────────────────────────────────────────────────────

Wielomiany wielu zmiennych są reprezentowana jako Python dictionary, gdzie kluczem jest krotka wykładników,
a wartością współczynnik:

```python
# f(x,y,z) = 4x^3*y^2 − 2xy^2*z + z^4 + 1
f = { (3,2,0): 4,  (1,2,1): -2,  (0,0,4): 1,  (0,0,0): 1 }
```

Zaimplementowane zostały również standardowe operacje takie jak dodawanie, odejmowanie, mnożenie itp.

// ──────────────────────────────────────────────────────────
= Lab. 4b — Porządki na jednomianach
// ──────────────────────────────────────────────────────────

Zaimplementowane zostały 3 porządki:

- *Lex* — porównujemy wykładniki od lewej, jak słowa
w słowniku.
- *Permutowany Lex* — to Lex z inną kolejnością zmiennych,
- *GradedLex (grlex)* — najpierw porównuje łączny stopień, przy remisie używa Lex.

```python
def lex_cmp(exp1, exp2, perm=None):
    if perm is not None:
        exp1 = tuple(exp1[i] for i in perm)
        exp2 = tuple(exp2[i] for i in perm)
    for a, b in zip(exp1, exp2):
        if a != b: return 1 if a > b else -1
    return 0

def grlex_cmp(exp1, exp2):
    d1, d2 = sum(exp1), sum(exp2)
    if d1 != d2: return 1 if d1 > d2 else -1
    return lex_cmp(exp1, exp2)
```

// ──────────────────────────────────────────────────────────
= Lab. 4c — Algorytm PolynomialReduce
// ──────────────────────────────────────────────────────────

Algorytm dzieli wielomian $f$ przez listę $G = (g_1, dots, g_n)$,
wyznaczając rozkład:
$ f = alpha_1 g_1 + alpha_2 g_2 + dots.c + alpha_n g_n + r $
gdzie reszta $r$ spełnia: żaden wyraz $r$ nie jest podzielny przez
$"LT"(g_i)$ dla żadnego $i$.

```python
def polynomial_reduce(f, G, cmp_fn):
    alphas = [{} for _ in range(len(G))]
    r = {}
    p = dict(f)

    while p:
        lt_exp, lt_coeff = leading_term(p, cmp_fn)
        divided = False

        for i, g in enumerate(G):
            g_lt_exp, g_lt_coeff = leading_term(g, cmp_fn)
            if divides(g_lt_exp, lt_exp):
                q_exp, q_coeff = monomial_div(
                    lt_exp, lt_coeff, g_lt_exp, g_lt_coeff)
                alphas[i][q_exp] = alphas[i].get(q_exp, 0) + q_coeff
                alphas[i] = poly_clean(alphas[i])
                p = poly_sub(p, poly_mul_monomial(g, q_exp, q_coeff))
                divided = True
                break

        if not divided:
            r[lt_exp] = r.get(lt_exp, 0) + lt_coeff
            del p[lt_exp]

    return alphas, poly_clean(r)
```

Jest to standardowy algorytm dzielenia "pisemnego".
1. wczytujemy wartości: p = f, $alpha_i = 0$ r = 0
2. wybieramy największy wyraz p
3. Dla każdego $g_i in G$ po kolei sprawdzamy czy $L T(g_i)$ dzieli $L T(p)$, jeśli tak to wyznaczamy odpowiedni mnożnik q
4. Dodajemy q do odpowiedniego $alpha_i$ i odejmujemy od p
5. Jeśli żaden $g_i$ nie spełnia wymogu to przenosimy to do reszty r i usuwamy z p
6. Kończymy gdy $p=0$


// ──────────────────────────────────────────────────────────
= Lab. 4d — Ćwiczenie 37
// ──────────────────────────────────────────────────────────

*Dane:* $f = x^3 - x^2y - x^2z$,#h(6pt)
$g_1 = x^2y - z$,#h(6pt)
$g_2 = x y - 1$,#h(6pt)
porządek GradedLex. Wyniki dzielenia:

#v(4pt)
#align(center)[
  #table(
    columns: (auto, auto, auto, auto),
    stroke: 0.4pt + luma(180),
    inset: (x: 8pt, y: 5pt),
    fill: (_, row) => if row == 0 { luma(225) } else { white },
    [*Kolejność*], [*$alpha_1$*], [*$alpha_2$*], [*Reszta $r$*],
    [$(g_1, g_2)$], [$-1$], [$0$], [$x^3 - x^2 z - z$],
    [$(g_2, g_1)$], [$-x$], [$0$], [$x^3 - x^2 z - x$],
  )
]
#v(4pt)


Różnice wynikają z tego że w kroku 2 obu redukcji $"LT"(p) = -x^2y$.
Algorytm próbuje dzielników po kolei:
- w $(g_1, g_2)$: $"LT"(g_1) = x^2y$ dzieli $-x^2y$ do $-1$;
- w $(g_2, g_1)$: $"LT"(g_2) = \x\y$ też dzieli $-x^2y$ do $-x$.

Ta różnica wpływa na ostatczne wartości.

*Czy $r_1 - r_2 in angle.l g_1, g_2 angle.r$?*
$r_1 - r_2 = x - z$.
Oba $"LT"(g_1) = x^2y$ i $"LT"(g_2) = \x\y$ mają wykładnik $y gt.eq 1$,
więc nie mogą dzielić żadnego wyrazu wielomianu $x - z$ (który nie
zawiera $y$). Zatem $r_1 - r_2 = x - z not in angle.l g_1, g_2 angle.r$.

// ──────────────────────────────────────────────────────────
= Lab. 4e — Wielomian własny
// ──────────────────────────────────────────────────────────


$ h(x,y,z) = x^2 y^7 - y^9 z^7 + z $


Wybrane 3 porządki i ciąg $G$:

Wybrane wielomiany:
- $g_1 = x^2y^7-z$
- $g_2 = y^9z^7-z$
- $g_3 = z - x$

Wybrane zostały 3 porządki - permutowany LEX:

- x > y > z
- z > x > y
- y > z > x




#v(4pt)
#align(center)[
  #table(
    columns: (auto, auto),
    stroke: 0.4pt + luma(180),
    inset: (x: 9pt, y: 5pt),
    fill: (_, row) => if row == 0 { luma(225) } else { white },
    [*Porządek*], [*Reszta $r$*],
    [$x > y > z$], [$z$],
    [$y > z > x$], [$x$],
    [$z > x > y$], [$x^2 y^7$],
  )
]
#v(4pt)

*Dlaczego wyniki są różne?*

Wielomiany $g_i$ zostały wybrane w taki sposób by w zależności od tego który współczynnik jest uznawany za największy "usunąć" inny monom z funkcji $h$ w trakcie wykonywania algorytmu. W ten sposób możemy sprawić że w zależności od wybranego porządku inna funkcja była wykorzystana do dzielenia, prowaząc do innej reszty. 



= Lab. 5 Godło Japonii
Do narysowania wykresów wykorzystałem matplotlib.
Wykorzystany wzór (we współrzędnych biegunowych):
$ r = abs(sin(8 theta))^(1/4) $
gdzie $ theta in [0,2 pi)$. Wzór jest inspirowany równaniem Quadrifolium z paroma zmianami:
- wartość bezwzględna - uniknięcie problemów z potęgowaniem
- $8 theta$ - by uzyskać 16 płatków
- $.^(1/4)$ - by przeskalować płatki - by były "grubsze"
Dodatkowo na środku narysowane jest koło.

W ten sposób otrzymujemy godło podobne do tego, które znajduje się na paszporcie japońskim. W celu dodania drugiego rzędu płatków starczy narysować najpierw w tle ten sam obraz ale przesunięty z wartością $theta$ przesuniętą o jakiś offset. Ja eksperymentalnie wyznaczyłem jako offset $pi/16$  


#align(center)[
  #grid(
    columns: 2,
    gutter: 1cm,
    image("../kamon_final.png", width: 100%),
    image("../kamon_full.png", width: 100%),
  )
]

*Krzywe Algebraiczne:*
Bierzemy wzór 
$r = abs(sin(8 theta))^(1/4)$ po podniesieniu do 8 potęgi otrzymujemy $r^8=sin^2(8 theta)$.
Mamy $(x+y i)^8=r^8(cos(8 theta) + i sin(8 theta))$, zatem $I m ((x+y i)^8)=r^8sin(8 theta)$, podnosimy do kwadratu i dostajemy
$r^16sin^2(8 theta) = I m ((x+y i)^8)^2$, $r^2 =x^2+y^2$ i $sin^2(8 theta) = r^8$ zatem $r^24=(x^2+y^2)^12$ Zatem ostatcznie równanie krzywej ma postać:
$ (x^2+y^2)^12 = I m ((x+y i)^8)^2 $
lub jako już wyliczony wielomian:
$ (x^2+y^2)^12-(8 x y(x-y)(x+y)(x^2-2 x y -y^2)(x^2+2 x y -y^2))^2 = 0 $

By dodać okrąg po środku wystarczy wziąć równanie okręgu:
$ x^2+y^2=r^2 $

I te równanie okręgu pomnożyć razy równanie "płatków". Wtedy otrzymamy pełny wykres.