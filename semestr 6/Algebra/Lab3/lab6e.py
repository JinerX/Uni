from fractions import Fraction
from math import gcd
from functools import reduce
import numpy as np
import matplotlib.pyplot as plt

from lab6a import lex_cmp, poly_clean, poly_str_ordered, leading_term
from lab6b import buchberger

b = 7

VAR4 = ["c", "s", "x", "y"]
VAR2 = ["x", "y"]


def fp(d):
    return {exp: Fraction(v) for exp, v in d.items() if v != 0}


def poly_normalize(p, cmp_fn):
    if not p:
        return {}
    _, lc = leading_term(p, cmp_fn)
    return poly_clean({exp: c / lc for exp, c in p.items()})


def has_param(poly):
    """True if polynomial involves c (index 0) or s (index 1)."""
    return any(exp[0] > 0 or exp[1] > 0 for exp in poly)


def strip_params(poly):
    """Convert 4-var poly (c_exp=s_exp=0) to 2-var poly in (x,y)."""
    return poly_clean({(exp[2], exp[3]): c for exp, c in poly.items()})


def scale_to_int(poly):
    """Scale Fraction-coefficient polynomial to integer coefficients."""
    if not poly:
        return {}
    lcm_denom = reduce(lambda a, b: a * b // gcd(a, b),
                       [c.denominator for c in poly.values()])
    return {exp: int(c * lcm_denom) for exp, c in poly.items()}


def eval_poly_xy(p, X, Y):
    result = np.zeros_like(X, dtype=float)
    for (ex, ey), c in p.items():
        result += float(c) * (X ** ex) * (Y ** ey)
    return result



f1 = fp({(2,0,0,0): -b, (0,0,1,0): 1, (0,0,0,0): -b})
f2 = fp({(1,0,0,1):  1, (0,1,1,0): -1})
f3 = fp({(2,0,0,0):  1, (0,2,0,0):  1, (0,0,0,0): -1})

print(f"Trysektrysa: r = {b}(cosθ + 1/cosθ)")
print("Układ parametryczny (c=cosθ, s=sinθ):")
print(f"  f1: x − {b}c² − {b} = 0")
print("  f2: yc − xs = 0")
print("  f3: c² + s² − 1 = 0")
print()

gb = buchberger([f1, f2, f3], lex_cmp)
gb_norm = [poly_normalize(p, lex_cmp) for p in gb]

print(f"Baza Groebnera ({len(gb_norm)} elem., lex c>s>x>y):")
for p in gb_norm:
    print(f"  {poly_str_ordered(p, lex_cmp, VAR4)}")

elim = [p for p in gb_norm if not has_param(p)]
print(f"\nRównanie kartezjańskie (bez c, s) – {len(elim)} elem.:")
elim_xy = None
for p in elim:
    p2 = strip_params(p)
    print(f"  {poly_str_ordered(p2, lex_cmp, VAR2)}")
    elim_xy = p2

if elim_xy:
    ip = scale_to_int(elim_xy)
    print(f"\nPo przeskalowaniu do liczb całkowitych:")
    print(f"  {poly_str_ordered(ip, lex_cmp, VAR2)}")

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 7))
fig.suptitle(
    f"Trysektrysa:  r = {b}(cosθ + 1/cosθ),  b={b}\n"
    f"Równanie:  x^3 − {2*b}x^2 + xy^2 − {b}y^2 = 0",
    fontsize=12,
)

YLim = 30
XLim_lo, XLim_hi = 0, 2*b + 2

ax1.set_title("Kontur wyznaczony", fontsize=10)
ax1.axhline(0, color="lightgray", lw=0.8)
ax1.axvline(0, color="lightgray", lw=0.8)
ax1.axvline(b, color="orange", lw=1.2, ls="--", label=f"asymptota x = {b}")
ax1.set_xlabel("x"); ax1.set_ylabel("y")
ax1.set_xlim(XLim_lo, XLim_hi)
ax1.set_ylim(-YLim, YLim)

if elim_xy:
    xg = np.linspace(b + 0.02, 2*b + 1.5, 800)
    yg = np.linspace(-YLim, YLim, 800)
    XG, YG = np.meshgrid(xg, yg)
    FG = eval_poly_xy(elim_xy, XG, YG)
    ax1.contour(XG, YG, FG, levels=[0], colors=["blue"])

ax1.legend(fontsize=9)

ax2.set_title("Parametryczna", fontsize=10)
ax2.axhline(0, color="lightgray", lw=0.8)
ax2.axvline(0, color="lightgray", lw=0.8)
ax2.axvline(b, color="orange", lw=1.2, ls="--", label=f"asymptota x = {b}")
ax2.set_xlabel("x"); ax2.set_ylabel("y")
ax2.set_xlim(XLim_lo, XLim_hi)
ax2.set_ylim(-YLim, YLim)

theta = np.linspace(-np.pi/2 + 0.005, np.pi/2 - 0.005, 3000)
ct = np.cos(theta)
xp = b * (ct**2 + 1)
yp = b * (ct**2 + 1) * np.sin(theta) / ct
ax2.plot(xp, yp, "b-", lw=1.5, label="trysektrysa")
ax2.legend(fontsize=9)

plt.tight_layout()
plt.savefig("lab6e_trisectrix.png", dpi=150)
plt.show()
print("\nWykres zapisany: lab6e_trisectrix.png")
