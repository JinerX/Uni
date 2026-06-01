from lab4a import poly_sub, poly_add, poly_mul
from lab4b import grlex_cmp, poly_str_ordered
from lab4c import polynomial_reduce

V = ["x", "y", "z"]

f  = {(3,0,0):  1, (2,1,0): -1, (2,0,1): -1}
g1 = {(2,1,0):  1, (0,0,1): -1}
g2 = {(1,1,0):  1, (0,0,0): -1}

def show(label, poly):
    print(f"  {label} = {poly_str_ordered(poly, grlex_cmp, V)}")

def verify(alphas, r, G, name):
    total = dict(r)
    for a, g in zip(alphas, G):
        total = poly_add(total, poly_mul(a, g))
    ok = total == f
    print(f"  sprawdzenie (suma = f): {'OK' if ok else 'BŁĄD'}")


print("GradLex\n")
print(f"f  = {poly_str_ordered(f,  grlex_cmp, V)}")
print(f"g1 = {poly_str_ordered(g1, grlex_cmp, V)}")
print(f"g2 = {poly_str_ordered(g2, grlex_cmp, V)}")

print("\nf / (g1, g2)")
alphas1, r1 = polynomial_reduce(f, [g1, g2], grlex_cmp)
show("alpha1", alphas1[0])
show("alpha2", alphas1[1])
show("r1    ", r1)
verify(alphas1, r1, [g1, g2], "(g1,g2)")

print("\nf / (g2, g1)")
alphas2, r2 = polynomial_reduce(f, [g2, g1], grlex_cmp)
show("alpha1", alphas2[0])
show("alpha2", alphas2[1])
show("r2    ", r2)
verify(alphas2, r2, [g2, g1], "(g2,g1)")

print("\nr1 - r2 in ideal")
diff = poly_sub(r1, r2)
show("r1 - r2", diff)

_, rem1 = polynomial_reduce(diff, [g1, g2], grlex_cmp)
_, rem2 = polynomial_reduce(diff, [g2, g1], grlex_cmp)
show("reszta z (r1-r2) / (g1,g2)", rem1)
show("reszta z (r1-r2) / (g2,g1)", rem2)
