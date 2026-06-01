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


def poly_str(f, var_names=None):
    if not f:
        return "0"
    n = len(next(iter(f)))
    if var_names is None:
        var_names = [chr(ord('x') + i) for i in range(min(n, 26))]

    def monomial_str(exp, coeff):
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

    terms = sorted(f.items(), key=lambda kv: (sum(kv[0]), kv[0]), reverse=True)
    parts = [monomial_str(exp, c) for exp, c in terms if c != 0]

    if not parts:
        return "0"

    result = parts[0]
    for s in parts[1:]:
        if s.startswith("-"):
            result += f" - {s[1:]}"
        else:
            result += f" + {s}"
    return result


