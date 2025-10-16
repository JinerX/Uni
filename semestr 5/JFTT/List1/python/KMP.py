import sys
import typing
import string

def get_lps(pattern: str):
    m = len(pattern)
    lps = [0]*m
    j = 0
    i = 1

    while i < m:
        if pattern[i] == pattern[j]:
            j += 1
            lps[i] = j
            i += 1
        else:
            if j == 0:
                lps[i] = 0
                i += 1
            else:
                j = lps[j-1]
    return lps

def KMP(text: str, pattern: str):
    lps = get_lps(pattern=pattern)
    i = 0
    j = 0
    m = len(lps)
    n = len(text)
    
    if m == 0:
        print("Empty pattern")
        return
    while i < n:
        if pattern[j] == text[i]: #match
            i += 1
            j += 1
            if j == m:
                print(f"match detected at index: {i-m}")
                j = lps[j-1]

        else:
            if j == 0:
                i += 1
            else:
                j = lps[j-1]

try:
    pattern = sys.argv[1]
    filename = sys.argv[2]
except:
    print("Incorrect input should be: python program_name <pattern> <file_name>")
    sys.exit()

with open(filename, "r") as f:
    text = f.read()

KMP(text=text, pattern=pattern)