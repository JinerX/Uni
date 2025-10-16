import string
import typing
import sys



default_alphabet = set(string.ascii_letters + string.punctuation + string.digits)

def create_delta(pattern: str, alphabet: set):
    m = len(pattern)
    delta = dict()
    for q in range(0,m+1):
        for letter in alphabet:
            k = min(q+1, m)
            while pattern[:k] != (pattern[q-k+1:q] + letter):
                k = k-1
                if k == 0:
                    break
            delta[(q,letter)] = k
    return delta

def scan_text(delta: dict, text: str, m: int):
    q = 0
    for i in range(len(text)):
        q = delta[q, text[i]]
        if q == m:
            print(f"match found at idx: {i-m+1}")


try:
    pattern = sys.argv[1]
    filename = sys.argv[2]
except:
    print("Incorrectly formatted input values should be: python program_name <pattern> <filename>")
    sys.exit()
print(pattern)

with open(filename, "r") as f:
    text = f.read()


alphabet = set(text) | set(pattern)


delta = create_delta(pattern=pattern, alphabet=alphabet)
scan_text(delta=delta, text=text, m=len(pattern))

