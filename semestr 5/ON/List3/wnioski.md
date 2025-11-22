## zad4
1. metoda bisekcji - error, brak zmiany znaku na przedziale [1.5,2]
2. szybka zbieżność, wynik poprawny, zbiega szybko ponieważ metoda newtona ma zbieżność kwadratową dgy
    -  punkt startowy jest blisko pierwiastka (tutaj jest)
    - pochodna blisko pierwiastka nie jest bliska 0 (tutaj nie jest)
3. metoda siecznych - zbieżność wolniej niż Newton i gorsze przybliżenie
    - gorsza zbieżność od metody Newtona (wykładnik (1+sqrt(5)))

## zad5
funkcja ma tylko 2 miejsca zerowe, uzasadnienie:
- g'(x) = e^x -3
- g''(x) = e^x

skoro g''(x) > 0 \forall x to g jest ściśle wypukła

skoro jest ściśle wypukła zakładamy nie wprost że ma 3 miejsca zerowe

wtedy prosta y = 0 musiałaby leżeć nad wartościami funkcji na całej długości, nie leży więc elo

potem tylko obliczamy wartości funkcji

## zad6
dla dobrych danych początkowych - blisko miejsca zerowego, działa wszystko w porządku, znajduje dobre miejsca zerowe

mstycznych:
1. f1
    - dla x0=2: jeszcze działa okej, dostatecznie stroma styczna (duża pochodna)
    - dla większych x0 wykładniczo zwiększa nam się liczba wymaganych iteracji, dla x0=10 już przekraczamy zakres Float64 (w obliczonej wartości funkcji nie liczbie iteracji)
    - powodem jest to że dla dużych wartości x0 f1(x) jest coraz bliżej prostej 1 (do której zbiega) przez co mamy bardzo małą wartość pochodnej a styczna zaczyna przypominać prostą równoległą do osi OX
    - dla x0=1 mamy od razu pochodną równą 0 nie ma żadnej zmiany wartości elementów ciągu
2. f2
    - dla wartości x0 > 1 ciąg "idzie" w złym kierunku, ponieważ funkcja f2 w nieskończoności zbiega do 0 to w końcu wartości ciągu są na tyle małe, że przekraczają precyzję arytmetyki i są zaokrąglane do 0 (około x\approx 14) i nasz algorytm aproksymacyjny "wykrywa" miejsce zerowe około x=14 którego tam tak naprawdę nie ma
