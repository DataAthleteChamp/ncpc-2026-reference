#!/usr/bin/env python3
"""Random test generator for stress testing.

Usage:  python3 gen.py SEED > test.in

The seed makes each run reproducible: when `make stress` reports a mismatch
on seed 47, `python3 gen.py 47` regenerates that exact failing test.

EDIT THIS FILE to match the input format of the problem you are solving.
Keep the numbers SMALL (n <= 8, values <= 10) -- the brute force has to be
fast, and a small counterexample is one you can actually read.
"""
import random
import sys

seed = int(sys.argv[1]) if len(sys.argv) > 1 else 0
random.seed(seed)

# ---- edit below to match the problem's input format ----------------------
n = random.randint(1, 8)
values = [random.randint(1, 10) for _ in range(n)]

print(n)
print(*values)
