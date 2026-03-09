import numpy as np
from math import sqrt
PATH = "data/dj38.tsp"

nodes = []

with open(PATH, 'r') as f:
    for line in f:
        words = line.split(" ")
        first_word = words[0]
        if (first_word.isdigit()):
            nodes.append((float(words[1]), float(words[2])))

        elif (first_word == "NAME:"):
            print(f"Hello {words[1]}")

for i,node in enumerate(nodes):
    print(i, node)

nodes = np.array(nodes)

perm = np.random.permutation(len(nodes))

for i in range(len(nodes)):
    first = i
    second = (i+1)%len(nodes)
    dist = sqrt((perm[first][0] - perm[second][0]) ** 2 + (perm[first][1] - perm[second][1]))
    print(perm[first], '\n', perm[second], '\n', dist)