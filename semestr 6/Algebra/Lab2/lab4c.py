from lab4a import poly_add, poly_sub, poly_mul_monomial, poly_clean
from lab4b import leading_term, poly_str_ordered


def divides(exp_divisor, exp_dividend):
    return all(a <= b for a, b in zip(exp_divisor, exp_dividend))


def monomial_div(exp_dividend, c_dividend, exp_divisor, c_divisor):
    exp = tuple(a - b for a, b in zip(exp_dividend, exp_divisor))
    return exp, c_dividend / c_divisor


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

