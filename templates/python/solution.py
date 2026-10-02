"""Python solution template for NCPC 2026.

THE JUDGE RUNS PYPY3 7.3.23 == Python 3.11.15, not CPython.

  * Do NOT use syntax newer than 3.11. Your local Python is 3.14 and will
    accept things the judge rejects.
  * numpy is slow or absent under PyPy. Plain loops are FAST under PyPy.
  * sys.stdin.buffer.read() is mandatory; input() is far too slow.

USE PYTHON WHEN: the problem is string/bignum/parsing heavy, or the logic is
fiddly and n is small. USE C++ WHEN: n is large or the limit is tight.

Local run:
    python3 sol.py < a.in > a.out
    LOCAL=1 python3 sol.py        # reads in.txt, writes out.txt
"""
import os
import sys

# Debug to stderr, never stdout -- stdout is the answer the judge reads.
DEBUG = os.environ.get("LOCAL") == "1"


def dbg(*args):
    if DEBUG:
        print(*args, file=sys.stderr)


def main():
    data = sys.stdin.buffer.read().split()
    pos = 0

    def nxt_int():
        nonlocal pos
        pos += 1
        return int(data[pos - 1])

    def nxt_str():
        nonlocal pos
        pos += 1
        return data[pos - 1].decode()

    out = []

    # ---- solve ----
    n = nxt_int()
    a = [nxt_int() for _ in range(n)]
    dbg("n =", n, "a =", a)
    out.append(str(sum(a)))
    # ---------------

    sys.stdout.write("\n".join(out) + "\n")


if DEBUG and os.path.exists("in.txt"):
    sys.stdin = open("in.txt")
    sys.stdout = open("out.txt", "w")

main()
