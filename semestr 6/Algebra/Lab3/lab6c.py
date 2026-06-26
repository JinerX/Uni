from fractions import Fraction
import numpy as np
import matplotlib.pyplot as plt

from lab6a import (
    poly_clean, poly_scale, leading_term,
    make_lex, lex_cmp, poly_str_ordered,
)
from lab6b import buchberger

a, b, c, d, e, f_const = 2, 7, 9, 7, 0, 1

lex_z_first = make_lex(perm=(2, 1, 0))

VAR3 = ["x", "y", "z"]
VAR2 = ["x", "y"]


def fp(d):
    return {exp: Fraction(v) for exp, v in d.items() if v != 0}


def poly_normalize(p, cmp_fn):
    if not p:
        return {}
    _, lc = leading_term(p, cmp_fn)
    return poly_clean({exp: c / lc for exp, c in p.items()})


def has_z(poly):
    return any(exp[2] > 0 for exp in poly)


def strip_z(poly):
    return poly_clean({(exp[0], exp[1]): c for exp, c in poly.items()})


def eval_poly2(p, X, Y):
    result = np.zeros_like(X, dtype=float)
    for (ex, ey), c in p.items():
        result += float(c) * (X ** ex) * (Y ** ey)
    return result



pythag = fp({(2, 0, 0): 1, (0, 2, 0): 1, (0, 0, 2): -1})

cases = [
    (f"(c+1)z  =  {c+1}z",
     fp({(0, 0, 1): c + 1})),

    (f"z + d + 1  =  z + {d+1}",
     fp({(0, 0, 1): 1, (0, 0, 0): d + 1})),

    (f"x + z − e − 1  =  x + z − {e+1}",
     fp({(1, 0, 0): 1, (0, 0, 1): 1, (0, 0, 0): -(e + 1)})),

    (f"x + y + z + f + 1  =  x + y + z + {f_const+1}",
     fp({(1, 0, 0): 1, (0, 1, 0): 1, (0, 0, 1): 1, (0, 0, 0): f_const + 1})),

    (f"y/a + z + 1  =  y/{a} + z + 1",
     {(0, 1, 0): Fraction(1, a), (0, 0, 1): Fraction(1), (0, 0, 0): Fraction(1)}),
]

all_elim = []

for name, f_lin in cases:
    print(f"\n{'='*60}")
    print(f"f(x,y,z) = {name}")

    gb = buchberger([pythag, f_lin], lex_z_first)
    gb_norm = [poly_normalize(p, lex_z_first) for p in gb]

    print(f"Baza Groebnera ({len(gb_norm)} elem.):")
    for p in gb_norm:
        print(f"  {poly_str_ordered(p, lex_z_first, VAR3)}")

    elim = [p for p in gb_norm if not has_z(p)]
    print(f"Ideał eliminacji I_f (bez z, {len(elim)} elem.):")
    for p in elim:
        s = strip_z(p)
        print(f"  {poly_str_ordered(s, lex_cmp, VAR2)}")

    all_elim.append((name, elim))

RANGE = 11
x_lin = np.linspace(-RANGE, RANGE, 800)
y_lin = np.linspace(-RANGE, RANGE, 800)
X, Y = np.meshgrid(x_lin, y_lin)

fig, axes = plt.subplots(2, 3, figsize=(15, 10))
fig.suptitle("krzywe - V(I_f)", fontsize=13)
axes = axes.flatten()

curve_descriptions = [
    f"10z=0 : z=0 : x^2+y^2=0\n",
    f"z+8=0 : z=−8 : x^2+y^2=64\n",
    f"x+z−1=0 : z=1−x : y^2+2x−1=0",
    f"x+y+z+2=0 : z=−x−y−2 : xy+2x+2y+2=0",
    f"y/2+z+1=0 : z=−y/2−1 : 4x^2+3y^2−4y−4=0",
]

for i, (name, elim) in enumerate(all_elim):
    ax = axes[i]
    ax.set_title(f"({i+1}) f = {name}", fontsize=8)
    ax.axhline(0, color="lightgray", linewidth=0.8)
    ax.axvline(0, color="lightgray", linewidth=0.8)
    ax.set_xlim(-RANGE, RANGE)
    ax.set_ylim(-RANGE, RANGE)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.text(0.02, 0.98, curve_descriptions[i], transform=ax.transAxes,
            fontsize=7, va="top", bbox=dict(boxstyle="round", fc="wheat", alpha=0.7))

    if i == 0:
        ax.plot(0, 0, "ro", markersize=8, label="(0,0)")
        ax.legend(fontsize=8)
    elif elim:
        for p in elim:
            s = strip_z(p)
            Z = eval_poly2(s, X, Y)
            cs = ax.contour(X, Y, Z, levels=[0], colors=["blue"])
    else:
        ax.text(0.5, 0.5, "brak krzywej", transform=ax.transAxes, ha="center")

axes[5].axis("off")

plt.tight_layout()
plt.savefig("lab6c_curves.png", dpi=150)
plt.show()
print("\nWykres zapisany: lab6c_curves.png")

