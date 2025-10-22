# Answears to questions from exercise 1

Jaki związek ma liczba macheps z precyzją arytmetyki $\epsilon$?

precyzja arytmetyki ($\epsilon$) to górne ograniczenie błędu względnego dla danej arytmetyki, machine epsilon ($\epsilon_{mach}$) to najmniejsza wartość taka że 1+macheps > 1.  

$\epsilon = \frac{1}{2}\epsilon_{mach}$



Jaki związek ma eta $\eta$ z liczbą $MIN_{sub}$?

W systemie IEEE-754 jest jej równa (bo w ogóle istnieją subnormalne). Jeśli mówimy o innym systemie gdzie liczby subnormalne nie koniecznie istnieją eta dalej istnieje.



Co zwracają funkcje `floatmin(Float32)` i `floatmin(Float64)` i jaki jest związek zwracanych wartości z liczbą $MIN_{nor}$?

zwracają najmniejszą dodatnią liczbę znormalizowaną w danym systemie liczbowym, są równe MIN_nor dla odpowiednio single i double w IEEE-754.