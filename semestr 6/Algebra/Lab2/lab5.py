
import numpy as np
import matplotlib.pyplot as plt
 
theta = np.linspace(0, 2*np.pi, 100000)
 
p = 0.25
 
r = np.abs(np.sin(8 * theta)) ** p
 
x = r * np.cos(theta)
y = r * np.sin(theta)
 
fig, ax = plt.subplots(figsize=(6, 6))
 
ax.fill(x, y, color='#f2c941', zorder=2)
ax.plot(x, y, lw=1.2, color='#000000', zorder=3)
 
# Centralny okrąg
circle_theta = np.linspace(0, 2*np.pi, 1000)
cr = 0.12
ax.fill(cr * np.cos(circle_theta), cr * np.sin(circle_theta), color='#f2c941', zorder=4)
ax.plot(cr * np.cos(circle_theta), cr * np.sin(circle_theta), color='#000000', lw=1.5, zorder=5)

ax.set_aspect('equal')
ax.axis('off')
 
plt.tight_layout()
plt.savefig("kamon_final.png", dpi=150, bbox_inches='tight', facecolor='#eeeeee')
print("done")
 