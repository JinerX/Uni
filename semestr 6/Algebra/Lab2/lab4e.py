from lab4b import make_lex, poly_str_ordered, grlex_cmp
from lab4c import polynomial_reduce

V = ["x", "y", "z"]


DIGITS = [2, 7, 9, 7, 0, 1]

ORDERINGS = [
    [0, 1, 2],
    [1, 2, 0],
    [2, 0, 1],
]


G = [
    {(0, 9, 7): 1, (0,0,1):-1},
    {(2, 7, 0): 1, (0,0,1): -1},
    {(0,0,1): 1, (1,0,0): -1},
]


PERM_NAMES = {
    (0,1,2): "x>y>z", (0,2,1): "x>z>y",
    (1,0,2): "y>x>z", (1,2,0): "y>z>x",
    (2,0,1): "z>x>y", (2,1,0): "z>y>x",
}

def make_h(digits):
    a, b, c, d, e, f = digits
    h = {}
    for exp, coeff in [((a,b,0), +1), ((0,c,d), -1), ((e,0,f), +1)]:
        h[exp] = h.get(exp, 0) + coeff
    return {k: v for k, v in h.items() if v != 0}


h = make_h(DIGITS)
lex_default = make_lex()

print(f"h = {poly_str_ordered(h, lex_default, V)}")
print()

remainders = []
for perm in ORDERINGS:
    name = PERM_NAMES[tuple(perm)]
    cmp_fn = make_lex(perm)
    alphas, r = polynomial_reduce(h, G, cmp_fn)
    r_str = poly_str_ordered(r, cmp_fn, V) if r else "0"
    print(f"{name}: r = {r_str}")
    remainders.append(tuple(sorted(r.items())))


cmp_fn = grlex_cmp
alphas, r = polynomial_reduce(h, G, cmp_fn)
r_str = poly_str_ordered(r, cmp_fn, V) if r else "0"
print(f"Graded lex: r = {r_str}")
remainders.append(tuple(sorted(r.items())))

print()
if len(set(remainders)) == 3:
    print("OK — 3 reszty są parami różne")
else:
    print(f"NIE — tylko {len(set(remainders))} różne reszty, zmień G lub porządki")
