import typing

gauss_number = tuple[int, int]

def gauss_norm(number: gauss_number):
    return number[0]**2 + number[1]**2

def gauss_sub(number1: gauss_number, number2: gauss_number):
    val = (number1[0] - number2[0], number1[1] - number2[1])
    return val


def gauss_add(number1: gauss_number, number2: gauss_number):
    val = (number1[0] + number2[0], number1[1] + number2[1])
    return val

def gauss_mult()




def gauss_div_rem(orig_numerator: gauss_number, orig_denominator: gauss_number):
    
    if orig_denominator[1] == 0:
        complex_denom = orig_denominator[0]
        complex_numer = orig_numerator
    else:
        complex_denom = gauss_norm(orig_denominator)
        a = orig_numerator[0]
        b = orig_numerator[1]
        c = orig_denominator[0]
        d = orig_denominator[1]
        complex_numer = (a*c+b*d,b*c-a*d)
    inter_value = (complex_numer[0]/complex_denom, complex_numer[1]/complex_denom)
    div = (round(inter_value[0]), round(inter_value[1]))
    rem = 