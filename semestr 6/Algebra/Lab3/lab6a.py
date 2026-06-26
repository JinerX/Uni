from functools import cmp_to_key



def poly_clean(f):
    return {exp: c for exp, c in f.items() if c != 0}


def poly_add(f, g):
    result = dict(f)
    for exp, c in g.items():
        result[exp] = result.get(exp, 0) + c
    return poly_clean(result)


def poly_sub(f, g):
    result = dict(f)
    for exp, c in g.items():
        result[exp] = result.get(exp, 0) - c
    return poly_clean(result)


def poly_mul(f, g):
    result = {}
    for e1, c1 in f.items():
        for e2, c2 in g.items():
            exp = tuple(a + b for a, b in zip(e1, e2))
            result[exp] = result.get(exp, 0) + c1 * c2
    return poly_clean(result)


def poly_mul_monomial(f, exp_m, coeff_m):
    result = {}
    for exp, c in f.items():
        new_exp = tuple(a + b for a, b in zip(exp, exp_m))
        result[new_exp] = c * coeff_m
    return poly_clean(result)


def poly_scale(f, scalar):
    return poly_clean({exp: c * scalar for exp, c in f.items()})


def poly_norm(f):
    return sum(abs(c) for c in f.values())


def poly_degree(f):
    if not f:
        return -1
    return max(sum(exp) for exp in f)


def poly_eval(f, values):
    result = 0
    for exp, c in f.items():
        term = c
        for e, v in zip(exp, values):
            term *= v ** e
        result += term
    return result



def lex_cmp(exp1, exp2, perm=None):
    if perm is not None:
        exp1 = tuple(exp1[i] for i in perm)
        exp2 = tuple(exp2[i] for i in perm)
    for a, b in zip(exp1, exp2):
        if a != b:
            return 1 if a > b else -1
    return 0


def grlex_cmp(exp1, exp2):
    d1, d2 = sum(exp1), sum(exp2)
    if d1 != d2:
        return 1 if d1 > d2 else -1
    return lex_cmp(exp1, exp2)


def make_lex(perm=None):
    return lambda e1, e2: lex_cmp(e1, e2, perm)



def leading_term(f, cmp_fn):
    return max(f.items(), key=lambda kv: cmp_to_key(cmp_fn)(kv[0]))


def sorted_terms(f, cmp_fn):
    return sorted(f.items(), key=lambda kv: cmp_to_key(cmp_fn)(kv[0]), reverse=True)



def _monomial_str(exp, coeff, var_names):
    vars_part = "".join(
        (var_names[i] if e == 1 else f"{var_names[i]}^{e}")
        for i, e in enumerate(exp) if e > 0
    )
    if not vars_part:
        return str(coeff)
    if coeff == 1:
        return vars_part
    if coeff == -1:
        return f"-{vars_part}"
    return f"{coeff}{vars_part}"


def poly_str(f, var_names=None):
    if not f:
        return "0"
    n = len(next(iter(f)))
    if var_names is None:
        var_names = [chr(ord('x') + i) for i in range(min(n, 26))]
    terms = sorted(f.items(), key=lambda kv: (sum(kv[0]), kv[0]), reverse=True)
    parts = [_monomial_str(exp, c, var_names) for exp, c in terms if c != 0]
    if not parts:
        return "0"
    result = parts[0]
    for s in parts[1:]:
        result += f" - {s[1:]}" if s.startswith("-") else f" + {s}"
    return result


def poly_str_ordered(f, cmp_fn, var_names=None):
    if not f:
        return "0"
    n = len(next(iter(f)))
    if var_names is None:
        var_names = [chr(ord('x') + i) for i in range(min(n, 26))]
    parts = [_monomial_str(exp, c, var_names) for exp, c in sorted_terms(f, cmp_fn)]
    result = parts[0]
    for s in parts[1:]:
        result += f" - {s[1:]}" if s.startswith("-") else f" + {s}"
    return result
