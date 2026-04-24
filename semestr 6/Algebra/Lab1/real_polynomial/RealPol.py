EPS = 1e-4

# Monom jest krotką wykładników, np. (2,) dla x^2
# Dla wielu zmiennych wystarczyłoby przechowywać dłuższe krotki, np. (2, 1, 0) dla x^2 + y
Monomial = tuple[int, ...]
Polynomial = dict[Monomial, float]


def mono(exp: int) -> Monomial:
    return (exp,)


def mono_mul(m1: Monomial, m2: Monomial) -> Monomial:
    return tuple(a + b for a, b in zip(m1, m2))


def mono_sub(m1: Monomial, m2: Monomial) -> Monomial:
    return tuple(a - b for a, b in zip(m1, m2))


def poly_clean(p: Polynomial) -> Polynomial:
    return {m: c for m, c in p.items() if abs(c) > EPS}


def poly_zero() -> Polynomial:
    return {}


def poly_const(c: float) -> Polynomial:
    return {} if abs(c) <= EPS else {mono(0): float(c)}


def poly_var() -> Polynomial:
    return {mono(1): 1.0}


def poly_term(c: float, exp: int) -> Polynomial:
    return {} if abs(c) <= EPS else {mono(exp): float(c)}


def poly_from_coeffs(coeffs):
    p = {}
    for exp, c in enumerate(coeffs):
        if abs(c) > EPS:
            p[mono(exp)] = float(c)
    return p


def poly_add(p: Polynomial, q: Polynomial) -> Polynomial:
    r = dict(p)
    for m, c in q.items():
        r[m] = r.get(m, 0.0) + c
    return poly_clean(r)


def poly_sub(p: Polynomial, q: Polynomial) -> Polynomial:
    r = dict(p)
    for m, c in q.items():
        r[m] = r.get(m, 0.0) - c
    return poly_clean(r)


def poly_scalar_mul(p: Polynomial, c: float) -> Polynomial:
    return poly_clean({m: a * c for m, a in p.items()})


def poly_mul(p: Polynomial, q: Polynomial) -> Polynomial:
    r = {}
    for m1, c1 in p.items():
        for m2, c2 in q.items():
            m = mono_mul(m1, m2)
            r[m] = r.get(m, 0.0) + c1 * c2
    return poly_clean(r)


def poly_is_zero(p: Polynomial) -> bool:
    return len(poly_clean(p)) == 0


def poly_degree(p: Polynomial) -> int:
    p = poly_clean(p)
    if not p:
        return -1
    return max(sum(m) for m in p.keys())


def poly_lead_term(p: Polynomial):
    p = poly_clean(p)
    if not p:
        return None, 0.0
    m = max(p.keys(), key=lambda mm: (sum(mm), mm))
    return m, p[m]


def poly_lead_coeff(p: Polynomial) -> float:
    return poly_lead_term(p)[1]


def poly_monic(p: Polynomial) -> Polynomial:
    p = poly_clean(p)
    if not p:
        return {}
    lc = poly_lead_coeff(p)
    return poly_scalar_mul(p, 1.0 / lc)


def poly_div_rem(dividend: Polynomial, divisor: Polynomial):
    dividend = poly_clean(dividend)
    divisor = poly_clean(divisor)

    if poly_is_zero(divisor):
        raise ValueError("Cannot divide by 0 polynomial")

    q = {}
    r = dict(dividend)

    lm_d, lc_d = poly_lead_term(divisor)
    deg_d = sum(lm_d)

    while not poly_is_zero(r):
        lm_r, lc_r = poly_lead_term(r)
        deg_r = sum(lm_r)

        if deg_r < deg_d:
            break

        if lm_r[0] < lm_d[0]:
            break

        t_m = mono_sub(lm_r, lm_d)
        t_c = lc_r / lc_d
        t = {t_m: t_c}

        q = poly_add(q, t)
        r = poly_sub(r, poly_mul(divisor, t))

    return poly_clean(q), poly_clean(r)


def poly_gcd(f: Polynomial, g: Polynomial) -> Polynomial:
    f = poly_clean(f)
    g = poly_clean(g)

    while not poly_is_zero(g):
        _, r = poly_div_rem(f, g)
        f, g = g, r

    return poly_monic(f)


def poly_xgcd(f: Polynomial, g: Polynomial):
    f = poly_clean(f)
    g = poly_clean(g)

    if poly_is_zero(f) and poly_is_zero(g):
        return {}, {}, {}

    if poly_is_zero(g):
        d = poly_monic(f)
        return d, poly_monic({mono(0): 1.0}), {}

    if poly_is_zero(f):
        d = poly_monic(g)
        return d, {}, poly_monic({mono(0): 1.0})

    r0, r1 = f, g
    s0, s1 = {mono(0): 1.0}, {}
    t0, t1 = {}, {mono(0): 1.0}

    while not poly_is_zero(r1):
        q, r = poly_div_rem(r0, r1)
        r0, r1 = r1, r
        s0, s1 = s1, poly_sub(s0, poly_mul(q, s1))
        t0, t1 = t1, poly_sub(t0, poly_mul(q, t1))

    lc = poly_lead_coeff(r0)
    inv = 1.0 / lc

    return (
        poly_scalar_mul(r0, inv),
        poly_scalar_mul(s0, inv),
        poly_scalar_mul(t0, inv),
    )


def poly_lcm(f: Polynomial, g: Polynomial) -> Polynomial:
    if poly_is_zero(f) or poly_is_zero(g):
        return {}

    d = poly_gcd(f, g)
    q, r = poly_div_rem(poly_mul(f, g), d)

    if not poly_is_zero(r):
        raise ValueError("LCM division should be exact")

    return poly_monic(q)


def poly_to_str(p):
    if not p:
        return "0"

    parts = []
    for m in sorted(p.keys(), key=lambda mm: sum(mm), reverse=True):
        c = p[m]
        exp = m[0]

        if exp == 0:
            term = f"{c:g}"
        elif exp == 1:
            if c == 1:
                term = "x"
            elif c == -1:
                term = "-x"
            else:
                term = f"{c:g}x"
        else:
            if c == 1:
                term = f"x^{exp}"
            elif c == -1:
                term = f"-x^{exp}"
            else:
                term = f"{c:g}x^{exp}"

        parts.append(term)

    s = " + ".join(parts)
    s = s.replace("+ -", "- ")
    return s