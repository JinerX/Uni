import gauss_ring.Gauss as G
import real_polynomial.RealPol as RP
import order.natural_ordering as NO



# gaussian integers are represented as tuples
# number a + bi is represented as (a,b)


a = 2
b = 7
c = 9
d = 7
e = 0
f = 1

# zad1

# b)
norm = G.gauss_norm((a,b))
print(f"Norm: {norm}")
results = G.gauss_div_rem_all((c+a,d+b), (e,f))
for i in results:
    print(f"div result {i}")

NWD = G.gauss_GCD_list_all([(a,b), (c,d), (e,d)])
NWW = G.gauss_LCM_list_all([(a,b), (c,d), (e,d)])

print(f"NWD: {NWD}")
print(f"NWW: {NWW}")

# zad2


p = RP.poly_add(RP.poly_term(c, a), RP.poly_const(b))
divisor = RP.poly_add(RP.poly_var(), RP.poly_const(1))

print("=== a) ===")
print(f"p(x) = {RP.poly_to_str(p)}")
print(f"norm(p) / degree = {RP.poly_degree(p)}")

q, r = RP.poly_div_rem(p, divisor)
print(f"p(x) / (x + 1) => q(x) = {RP.poly_to_str(q)}, r(x) = {RP.poly_to_str(r)}")
print()

# b) rozszerzony NWD i NWW
# v(x) = ax^3 + bx^2 + d
# w(x) = dx^3 + ex^2 + fx
#
# wybrano d = 0 oraz g = 0, żeby gcd nie było równe 1
g = 372.6000597


v = RP.poly_add(RP.poly_add(RP.poly_term(a, 3), RP.poly_term(b, 2)), RP.poly_const(d))
w = RP.poly_add(
    RP.poly_add(RP.poly_add(RP.poly_term(d, 3), RP.poly_term(e, 2)), RP.poly_term(f, 1)),
    RP.poly_const(g)
)

print("=== b) ===")
print(f"v(x) = {RP.poly_to_str(v)}")
print(f"w(x) + g = {RP.poly_to_str(w)}")

gcd, s, t = RP.poly_xgcd(v, w)
lcm = RP.poly_lcm(v, w)

print(f"gcd(v, w+g) = {RP.poly_to_str(gcd)}")
print(f"s(x) = {RP.poly_to_str(s)}")
print(f"t(x) = {RP.poly_to_str(t)}")
print(f"lcm(v, w+g) = {RP.poly_to_str(lcm)}")

check = RP.poly_add(RP.poly_mul(s, v), RP.poly_mul(t, w))
print(f"s(x)*v(x) + t(x)*(w(x)+g) = {RP.poly_to_str(check)}")

v_test = RP.poly_from_coeffs([-6, 1, 1]) 
# w(x) = x^2 - x - 2
w_test = RP.poly_from_coeffs([-2, -1, 1])

gcd_test = RP.poly_gcd(v_test, w_test)
print("=== TEST DOKŁADNY ===")
print(f"v(x) = {RP.poly_to_str(v_test)}")
print(f"w(x) = {RP.poly_to_str(w_test)}")
print(f"NWD(v, w) = {RP.poly_to_str(gcd_test)}") # Powinno wyjść x - 2

# zad3

print("ZAD 3===============================")
print("=== a) porównanie par w N^2 ===")
pairs = [(a, b), (c, d), (e, f)]
for p in pairs:
    for q in pairs:
        print(f"{p} <= {q}: {NO.product_le(p, q)}")
print()

print("=== b) porównanie trójek w N^3 ===")
triples = [(a, c, e), (b, d, f)]
for p in triples:
    for q in triples:
        print(f"{p} <= {q}: {NO.product_le(p, q)}")
print()

print("=== minimalne elementy zbioru A ===")
A_min = NO.minimal_elements_in_A(a, b)
print(f"A_min = {A_min}")
print()

print("=== minimalne elementy zbioru B ===")
B_min = NO.minimal_elements_in_B(c, d, e, f)
print(f"liczba elementów minimalnych w B: {len(B_min)}")
print(f"B_min = {B_min}")