# Jędrzej Sajnóg 279701


import matplotlib.pyplot as plt
from math import exp, log
import numpy as np

def f(x):
    return exp(x) * log(1 + exp(-x))



# X = range(0,50,0.01)
X = np.arange(0,50,0.01)
Y = [f(x) for x in X]

plt.plot(X,Y)
plt.savefig("python.png")
plt.show()