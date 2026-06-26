from fractions import Fraction
import numpy as np
import matplotlib.pyplot as plt

from lab6a import lex_cmp, poly_clean, poly_str_ordered, leading_term
from lab6b import buchberger

a = 2

VAR3 = ["x", "y", "z"]
VAR2_YZ = ["y", "z"]


def fp(d):
    return {exp: Fraction(v) for exp, v in d.items() if v != 0}


def poly_normalize(p, cmp_fn):
    if not p:
        return {}
    _, lc = leading_term(p, cmp_fn)
    return poly_clean({exp: c / lc for exp, c in p.items()})


def has_x(poly):
    return any(exp[0] > 0 for exp in poly)


def strip_x(poly):
    """Convert 3-var poly (x_exp=0 everywhere) to 2-var poly in (y,z)."""
    return poly_clean({(exp[1], exp[2]): c for exp, c in poly.items()})


def eval_poly2(p, A, B):
    """Evaluate 2-variable polynomial on numpy grids A, B."""
    result = np.zeros_like(A, dtype=float)
    for (ea, eb), c in p.items():
        result += float(c) * (A ** ea) * (B ** eb)
    return result


p1 = fp({
    (4, 0, 0):  1,
    (2, 2, 0):  2,
    (0, 4, 0):  1,
    (3, 0, 0): -4,
    (1, 2, 0): -4,
    (2, 0, 0):  4,
    (2, 0, 2): -1,
    (0, 2, 2): -1,
})

p2 = fp({(1, 0, 0): 1, (0, 1, 0): 2, (0, 0, 1): 3})


print("Układ:")
print(f"  p1 = (x^2+y^2−{a}x)^2 − z^2(x^2+y^2)")
print(f"  p2 = x + 2y + 3z")
print()

gb = buchberger([p1, p2], lex_cmp)
gb_norm = [poly_normalize(p, lex_cmp) for p in gb]

print(f"Baza Groebnera ({len(gb_norm)} elem., lex x>y>z):")
for p in gb_norm:
    print(f"  {poly_str_ordered(p, lex_cmp, VAR3)}")

elim = [p for p in gb_norm if not has_x(p)]
print(f"\nIdeał eliminacji I_x = I ∩ R[y,z] ({len(elim)} elem.):")
for p in elim:
    s = strip_x(p)
    print(f"  {poly_str_ordered(s, lex_cmp, VAR2_YZ)}")


R = 10
y_lin = np.linspace(-R, R, 800)
z_lin = np.linspace(-R, R, 800)
YG, ZG = np.meshgrid(y_lin, z_lin)

fig, axes = plt.subplots(1, 2, figsize=(14, 6))
fig.suptitle(
    f"Lab 6d – układ (x^2+y^2−{a}x)^2=z^2(x^2+y^2), x+2y+3z=0  (a={a})",
    fontsize=12,
)

ax1 = axes[0]
ax1.set_title("V(I_x): krzywa w płaszczyźnie (y, z)", fontsize=10)
ax1.axhline(0, color="lightgray", linewidth=0.8)
ax1.axvline(0, color="lightgray", linewidth=0.8)
ax1.set_xlabel("y")
ax1.set_ylabel("z")
ax1.set_xlim(-R, R)
ax1.set_ylim(-R, R)

if elim:
    s = strip_x(elim[0])
    FG = eval_poly2(s, YG, ZG)
    ax1.contour(YG, ZG, FG, levels=[0], colors=["blue"])
    ax1.set_title("V(I_x): krzywa eliminacji x w (y,z)", fontsize=10)


ax2 = axes[1]
ax2.set_title(f"z=a: kardioda r={a}(1+cosθ)", fontsize=10)
ax2.axhline(0, color="lightgray", linewidth=0.8)
ax2.axvline(0, color="lightgray", linewidth=0.8)
ax2.set_xlabel("x")
ax2.set_ylabel("y")
ax2.set_aspect("equal")

p1_z2 = fp({
    (4, 0, 0):  1,
    (2, 2, 0):  2,
    (0, 4, 0):  1,
    (3, 0, 0): -4,
    (1, 2, 0): -4,
    (2, 0, 0):  4 - a**2,
    (0, 2, 0): -a**2,
})
p1_z2_clean = fp({
    (4, 0, 0):  1,
    (2, 2, 0):  2,
    (0, 4, 0):  1,
    (3, 0, 0): -4,
    (1, 2, 0): -4,
    (2, 0, 0):  4 - a**2,
    (0, 2, 0): -a**2,
})

RC = 5
xc = np.linspace(-RC, RC, 800)
yc = np.linspace(-RC, RC, 800)
XC, YC = np.meshgrid(xc, yc)

F2 = (XC**2 + YC**2 - a*XC)**2 - a**2 * (XC**2 + YC**2)
ax2.contour(XC, YC, F2, levels=[0], colors=["crimson"])

theta = np.linspace(0, 2*np.pi, 1000)
r = a * (1 + np.cos(theta))
ax2.plot(r * np.cos(theta), r * np.sin(theta), "b--", linewidth=1.2,
         label=f"r={a}(1+cosθ) parametrycznie")
ax2.set_xlim(-RC, RC)
ax2.set_ylim(-RC, RC)
ax2.legend(fontsize=8)

plt.tight_layout()
plt.savefig("lab6d_plots.png", dpi=150)
plt.show()
print("\nWykres zapisany: lab6d_plots.png")

