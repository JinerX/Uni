import typing

gauss_number = tuple[int, int]

def gauss_norm(number: gauss_number):
    return number[0]**2 + number[1]**2

def gauss_sub(number1: gauss_number, number2: gauss_number):
    val = (number1[0] - number2[0], number1[1] - number2[1])
    return val

def gauss_associates(z: gauss_number):
    a, b = z
    candidates = [
        (a, b),
        (-a, -b),
        (-b, a),
        (b, -a),
    ]

    unique = []
    for x in candidates:
        if x not in unique:
            unique.append(x)
    return unique



def gauss_add(number1: gauss_number, number2: gauss_number):
    val = (number1[0] + number2[0], number1[1] + number2[1])
    return val

def gauss_mult(number1: gauss_number, number2: gauss_number):
    a = number1[0]
    b = number1[1]
    c = number2[0]
    d = number2[1]
    
    val = (a*c-b*d, a*d+b*c)
    return val

def int_floor(x):
    i = int(x)
    if x >= 0 or x == i:
        return i
    return i - 1

def int_ceil(x):
    i = int(x)
    if x <= 0 or x == i:
        return i
    return i + 1

def nearest_int_candidates(x):
    low = int_floor(x)
    high = int_ceil(x)

    if low == high:
        return [low]

    dist_low = abs(x - low)
    dist_high = abs(high - x)

    eps = 1e-12
    if dist_low + eps < dist_high:
        return [low]
    if dist_high + eps < dist_low:
        return [high]

    return [low, high]

def gauss_div_rem_all(orig_numerator: gauss_number, orig_denominator: gauss_number):
    if orig_denominator == (0, 0):
        raise ValueError("Cannot divide by 0")

    a = orig_numerator[0]
    b = orig_numerator[1]
    c = orig_denominator[0]
    d = orig_denominator[1]

    denom = c*c + d*d
    real = (a*c + b*d) / denom
    imag = (b*c - a*d) / denom

    real_candidates = nearest_int_candidates(real)
    imag_candidates = nearest_int_candidates(imag)

    results = []

    for x in real_candidates:
        for y in imag_candidates:
            div = (x, y)
            rem = gauss_sub(orig_numerator, gauss_mult(orig_denominator, div))
            if (div, rem) not in results:
                results.append((div, rem))

    return results

def gauss_div_rem(orig_numerator: gauss_number, orig_denominator: gauss_number):
    return gauss_div_rem_all(orig_numerator, orig_denominator)[0]

def gauss_GCD_list(numbers):
    numbers = list(numbers)

    if len(numbers) == 0:
        return (0, 0)
    if len(numbers) == 1:
        return numbers[0]

    result = numbers[0]
    for n in numbers[1:]:
        result = gauss_GCD(result, n)
    return result

def gauss_GCD_list_all(numbers):
    gcd = gauss_GCD_list(numbers)
    return gauss_associates(gcd)


def gauss_GCD(number1: gauss_number, number2: gauss_number):
    # algorytm Euklidesa
    if number1 == (0, 0):
        return number2
    if number2 == (0, 0):
        return number1

    while number2 != (0, 0):
        _, rem = gauss_div_rem(number1, number2)
        number1, number2 = number2, rem

    return number1


def gauss_LCM(number1: gauss_number, number2: gauss_number):
    # NWW = a*b / NWD
    if number1 == (0, 0) or number2 == (0, 0):
        return (0, 0)

    gcd = gauss_GCD(number1, number2)
    prod = gauss_mult(number1, number2)
    div, rem = gauss_div_rem(prod, gcd)

    if rem != (0, 0):
        raise ValueError("LCM should divide exactly")

    return div

def gauss_LCM_list(numbers):
    numbers = list(numbers)

    if len(numbers) == 0:
        return (1, 0)
    if len(numbers) == 1:
        return numbers[0]

    result = numbers[0]
    for n in numbers[1:]:
        result = gauss_LCM(result, n)
    return result

def gauss_LCM_list_all(numbers):
    lcm = gauss_LCM_list(numbers)
    return gauss_associates(lcm)