import numpy as np
import matplotlib.pyplot as plt

theta = np.linspace(0, 2*np.pi, 200000)
p = 0.25

# tylne płatki - offset, trochę większe
offset = np.pi / 16
r_back = np.abs(np.sin(8 * (theta - offset))) ** p * 1.02

x_back = r_back * np.cos(theta)
y_back = r_back * np.sin(theta)

# przednie płatki
r_front = np.abs(np.sin(8 * theta)) ** p

x_front = r_front * np.cos(theta)
y_front = r_front * np.sin(theta)

fig, ax = plt.subplots(figsize=(6, 6))

# tło
ax.fill(x_back, y_back, color='#f2c941', zorder=1)
ax.plot(x_back, y_back, lw=1.0, color='#000000', zorder=2)

# wierzch
ax.fill(x_front, y_front, color='#f2c941', zorder=3)
ax.plot(x_front, y_front, lw=1.2, color='#000000', zorder=4)

# okrąg
ct = np.linspace(0, 2*np.pi, 1000)
cr = 0.12
ax.fill(cr*np.cos(ct), cr*np.sin(ct), color='#f2c941', zorder=5)
ax.plot(cr*np.cos(ct), cr*np.sin(ct), color='#000000', lw=1.5, zorder=6)

ax.set_aspect('equal')
ax.axis('off')
plt.tight_layout()
plt.savefig("kamon_full.png", dpi=150,
            bbox_inches='tight', facecolor='#eeeeee')
print("done")