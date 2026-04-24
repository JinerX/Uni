def product_le(u: tuple[int, ...], v: tuple[int, ...]):
    if len(u) != len(v):
        raise ValueError("Tuples must have the same dimension")
    return all(ui <= vi for ui, vi in zip(u, v))


def product_lt(u: tuple[int, ...], v: tuple[int, ...]):
    return product_le(u, v) and u != v


def find_minimal_elements(elements: list[tuple[int, ...]]):
    minima = []

    for x in elements:
        is_minimal = True
        for y in elements:
            if y != x and product_le(y, x):
                is_minimal = False
                break

        if is_minimal and x not in minima:
            minima.append(x)

    return minima


def compare_pairs(p1: tuple[int, int], p2: tuple[int, int]):
    return product_le(p1, p2)


def compare_triples(p1: tuple[int, int, int], p2: tuple[int, int, int]):
    return product_le(p1, p2)


def points_in_A(a: int, b: int):
    points = []
    for x in range(max(0, a - 2), a + 3):
        for y in range(max(0, b - 2), b + 3):
            if (x - a) ** 2 + (y - b) ** 2 < 5:
                points.append((x, y))
    return points

def minimal_elements_in_B(c: int, d: int, e: int, f: int):
    minima = []
    for x1 in range(c + 16):
        for x2 in range(d + 16):
            for x3 in range(e + 16):
                for x4 in range(f + 16):
                    if (x1 - c)**2 + (x2 - d)**2 + (x3 - e)**2 + (x4 - f)**2 > 224:
                        is_minimal = True
                        for i in range(4):
                            p = [x1, x2, x3, x4]
                            if p[i] > 0:
                                p[i] -= 1
                                if (p[0] - c)**2 + (p[1] - d)**2 + (p[2] - e)**2 + (p[3] - f)**2 > 224:
                                    is_minimal = False
                                    break
                        if is_minimal:
                            minima.append((x1, x2, x3, x4))
    return minima


def minimal_elements_in_A(a: int, b: int):
    return find_minimal_elements(points_in_A(a, b))


def points_in_B(c: int, d: int, e: int, f: int):
    points = []

    x1_max = c + 15
    x2_max = d + 15
    x3_max = e + 15
    x4_max = f + 15

    for x1 in range(0, x1_max + 1):
        for x2 in range(0, x2_max + 1):
            for x3 in range(0, x3_max + 1):
                for x4 in range(0, x4_max + 1):
                    if (x1 - c) ** 2 + (x2 - d) ** 2 + (x3 - e) ** 2 + (x4 - f) ** 2 > 224:
                        points.append((x1, x2, x3, x4))

    return points

