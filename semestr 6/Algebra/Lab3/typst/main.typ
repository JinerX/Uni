#set document(title: "Sprawozdanie 3 - Wybrane zaganienia algebry")
#set page(paper: "a4", margin: (x: 2.5cm, y: 2.5cm))
#set text(size: 11pt, lang: "pl")
#set par(justify: true, leading: 0.7em)
#show heading.where(level: 1): it => [
  #v(0.9em)
  #it
  #v(0.35em)
]
#show heading.where(level: 2): it => [
  #v(0.6em)
  #it
  #v(0.2em)
]
#show raw.where(block: true): it => block(
  fill: luma(245), inset: 10pt, radius: 4pt, width: 100%,
)[#text(size: 8.5pt)[#it]]

// ── Strona tytułowa ──────────────────────────────────────────────────
#align(center)[
  #v(0.5em)
  #text(size: 17pt, weight: "bold")[Sprawozdanie 3 - Wybrane zagadnienia algebry]
  #v(0.25em)
  #text(size: 10pt)[
    Jędrzej Sajnóg, indeks:279701 
  ]
  #v(1.5em)
]

// ─────────────────────────────────────────────────────────────────────
= a) Pierścień wielomianów $RR[x_1, dots, x_n]$

Zaimplementowany analogicznie do Listy 2 - python dictionary gdzie key - krotka, value - współczynnik.

np.
```python
# w(x,y,z) = 5*x^3z^2 - 2*x^2y^2z+3z+5
w = {
  (3,0,2) : 5,
  (2,2,1) : -2,
  (0,0,1) : 3,
  (0,0,0) : 5
}
```

Zaimplementowane są też standardowe operacje: dodawanie, odejmowanie, mnożenie itp.

// ─────────────────────────────────────────────────────────────────────
= b) PolynomialReduce, Syzygium, algorytm Buchbergera

== PolynomialReduce

Tak samo jak na Liście drugiej.
Funkcja dzieli wielomian $f$ przez listę $G = [g_1, dots, g_k]$ w zadanym
porządku, zwracając współczynniki $[alpha_1, dots, alpha_k]$ i resztę
$r$ spełniające $f = sum_i alpha_i g_i + r$.

```python
def polynomial_reduce(f, G, cmp_fn):
    n = len(G)
    alphas = [{} for _ in range(n)]
    r = {}
    p = dict(f)
    while p:
        lt_exp, lt_coeff = leading_term(p, cmp_fn)
        divided = False
        for i, g in enumerate(G):
            g_lt_exp, g_lt_coeff = leading_term(g, cmp_fn)
            if divides(g_lt_exp, lt_exp):
                q_exp, q_coeff = monomial_div(lt_exp, lt_coeff, g_lt_exp, g_lt_coeff)
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

== Syzygium – S-wielomian

Eliminuje wiodące wyrazy pary $(f,g)$:
$
S(f,g) = frac(bold(x)^(l - "LM"(f)), "LC"(f)) dot f
        - frac(bold(x)^(l - "LM"(g)), "LC"(g)) dot g,
quad l = "LCM"("LM"(f),"LM"(g))
$
Gdzie:
- $L M$ - Leading monomial
- $L C$ - Leading coefficient
- $l$   - najmniejsza wspólna wielokrotność monomów głównych

```python
def s_polynomial(f, g, cmp_fn):
    lm_f, lc_f = leading_term(f, cmp_fn)
    lm_g, lc_g = leading_term(g, cmp_fn)
    lcm = lcm_mono(lm_f, lm_g)
    m_f = tuple(a - b for a, b in zip(lcm, lm_f))
    m_g = tuple(a - b for a, b in zip(lcm, lm_g))
    term_f = poly_mul_monomial(f, m_f, 1 / lc_f)
    term_g = poly_mul_monomial(g, m_g, 1 / lc_g)
    return poly_sub(term_f, term_g)
```

+ Wyznacz $"LM"(f)$, $"LC"(f)$, $"LM"(g)$, $"LC"(g)$ względem porządku `cmp_fn`.
+ Oblicz $l = "LCM"("LM"(f),"LM"(g))$ – maksimum wykładników po każdej zmiennej.
+ Wyznacz $m_f = l - "LM"(f)$ oraz $m_g = l - "LM"(g)$.
+ Zwróć $bold(x)^(m_f) / "LC"(f) dot f - bold(x)^(m_g) / "LC"(g) dot g$.

== Algorytm Buchbergera

```python
def buchberger(G, cmp_fn):
    basis = list(G)
    pairs = [(i, j) for i in range(len(basis)) for j in range(i + 1, len(basis))]
    while pairs:
        i, j = pairs.pop(0)
        s = s_polynomial(basis[i], basis[j], cmp_fn)
        _, r = polynomial_reduce(s, basis, cmp_fn)
        if r:
            new_idx = len(basis)
            basis.append(r)
            for k in range(new_idx):
                pairs.append((k, new_idx))
    return basis
```


+ Zainicjuj bazę $B arrow.l G$ i kolejkę par $P arrow.l brace.l (i,j) : i < j brace.r$.
+ Dopóki $P eq.not emptyset$:
  + Pobierz parę $(i,j)$ z $P$.
  + Oblicz Syzygium: $s arrow.l S(B_i, B_j)$.
  + Zredukuj: reszta $r arrow.l "PolynomialReduce"(s, B, "cmp_fn")$.
  + Jeśli $r eq.not 0$: dodaj $r$ do $B$, oznacz jego indeks $k_"new"$,
    dla każdego $k < k_"new"$ dodaj $(k, k_"new")$ do $P$.
+ Zwróć $B$ – baza Gröbnera ideału generowanego przez $G$.

// ─────────────────────────────────────────────────────────────────────
= c) Eliminacja $z$ – krzywe $V(I_f)$

Metoda eliminacji: Dla każdego układu ($x^2+y^2=z^2$ oraz $f_i (x,y,z)=0$)
obliczamy bazę Grobnera w porządku leksykograficznym $z > y > x$.
Z twierdzenia o eliminacji wiemy, że elementy bazy nieposiadające zmiennej $z$
generują ideał eliminacji $I_f = I inter RR[x,y]$. Na wykresach poniżej ukazane są wykresy $V(I_f)$

#figure(
  image("lab6c_curves.png", width: 100%),
  caption: [
    Krzywe $V(I_f)$ dla pięciu wielomianów $f_i$.
  ]
)

*Kształty krzywych – przekroje stożkowe.* Równanie $x^2+y^2=z^2$ opisuje
stożek obrotu w $RR^3$ z osią $z$, każda płaszczyzna $f_i=0$ tnie go zależnie od kąta nachylenia:

+ ($10z=0$, płaszczyzna pozioma przez wierzchołek):
  $x^2+y^2=0$ – jedynym rozwiązaniem $RR$ jest punkt $(0,0)$.

+ ($z=-8$, płaszczyzna pozioma): $x^2+y^2=64$ –
  okrąg o promieniu $r=8$.

+ ($z=1-x$, płaszczyzna nachylona równolegle do tworzącej
  stożka): $y^2+2x-1=0$ – parabola.

+ ($z=-x-y-2$, płaszczyzna nachylona stromiej niż tworząca):
  $(x+2)(y+2)=2$ – hiperbola.

+ ($z=-y/2-1$, płaszczyzna skośna):
  $4x^2+3y^2-4y-4=0$ – elipsa.

// ─────────────────────────────────────────────────────────────────────
= d) Układ limacon

$
cases(
  (x^2+y^2-2x)^2 = z^2(x^2+y^2),
  x+2y+3z = 0
)
$

Baza Grobnera w porządku $x>y>z$ zawiera trzy elementy,
jedynym bez $x$ jest wielomian wynikający z podstawienia
$x=-2y-3z$ do pierwszego równania:
$
  25y^4 + 120y^3 z + 40y^3 + 229y^2 z^2 + 156y^2 z + 16y^2 \
  + 204y z^3 + 216y z^2 + 48y z + 72z^4 + 108z^3 + 36z^2 = 0
$

#figure(
  image("lab6d_plots.png", width: 100%),
  caption: [
    Po lewej: $V(I_x)$ – krzywa eliminacji $x$.
    Po prawej: kardioda $r=2(1+cos theta)$ – pierwsze równanie dla $z=a=2$.
  ]
)

Lewy wykres to rzut krzywej wynikającej z przecięcia krzywej $(x^2+y^2-2x)^2 = z^2(x^2+y^2)$
z płaszczyzną $x+2y+3z=0$ na płaszczyznę $(y,z)$.

Prawy wykres to kardioda: podstawiając $z=a=2$ do pierwszego równania
i przechodząc do układu biegunowego, dostajemy
$(r-2cos theta)^2=4$, czyli $r=2(1+cos theta)$, co jest wzorem kardiody.

// ─────────────────────────────────────────────────────────────────────
= e) Trysektrysa

Wzór biegunowy: $r = b(cos theta + 1 slash cos theta)$, $b=7$.

Wyprowadzenie wzoru: Podstawiamy $c=cos theta$,
$s=sin theta$ i budujemy układ równań:
$
cases(
  x - b(c^2+1) = 0,
  y c - x s = 0,
  c^2 + s^2 - 1 = 0
)
$
gdzie pierwsze równanie pochodzi z $x=r cos theta = b(c^2+1)$, drugie
z $y slash x = sin theta slash cos theta = s slash c$, a trzecie to jedynka trygonometryczna. Tworzymy bazę Grobnera w porządku $c>s>x>y$
i zwracamy jedyny element bez $c$ i $s$:
$ x^3 - 14 x^2 + x y^2 - 7 y^2 = 0 $


#figure(
  image("lab6e_trisectrix.png", width: 100%),
  caption: [
    Trysektrysa: kontur $x^3-14x^2+x y^2-7y^2=0$ - wyznaczony (lewo) oraz
    krzywa bezpośrednio z wzoru biegunowego (prawo). Pionowa asymptota w $x=b=7$
  ]
)

Zastosowanie: Trysektrysa jest krzywą umożliwiającą
geometryczny trójpodział dowolnego kąta. Używając trysektrys można dla dowolnego kąta $phi$ skonstruować (narysować) kąt $phi/3$.

Sposób:
Zakładając że mamy kąt $angle A O B = phi$ 
+ ustaw punkty $O=(0,0)$, $A=(a,0)$
+ narysuj trysektyzę
+ narysuj w punkcie $A$ półprostą $L$ która z osią $x$ tworzy kąt $phi$
+ znajdź przecięcie prostej $L$ z trysektyzą - punkt $Q$
+ Połącz $O$ z $Q$
+ kąt między osią $x$ a półprostą $O Q$ wynosi $phi/3$
