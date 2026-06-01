from functools import cmp_to_key
from lab4a import poly_str



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


def poly_str_ordered(f, cmp_fn, var_names=None):
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

    parts = [monomial_str(exp, c) for exp, c in sorted_terms(f, cmp_fn)]
    result = parts[0]
    for s in parts[1:]:
        if s.startswith("-"):
            result += f" - {s[1:]}"
        else:
            result += f" + {s}"
    return result

