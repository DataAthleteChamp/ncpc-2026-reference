"""Python template for ad-hoc NCPC problems.

THE JUDGE RUNS PYPY3 7.3.23 == Python 3.11.15, not CPython.

Consequences you must respect:
  * Do NOT use syntax newer than 3.11. Your local Python 3.14 will happily
    accept things the judge rejects.
  * numpy is slow or unavailable under PyPy. Plain loops are FAST under PyPy.
  * sys.stdin.readline (or the bulk read below) is mandatory. input() is slow.

USE PYTHON WHEN: the problem is string/bignum/parsing-heavy, or the logic is
fiddly and n is small. USE C++ WHEN: n is large or the time limit is tight.
"""
import sys


def main() -> None:
    data = sys.stdin.buffer.read().split()
    pos = 0

    def nxt() -> int:
        nonlocal pos
        pos += 1
        return int(data[pos - 1])

    n = nxt()
    a = [nxt() for _ in range(n)]

    sys.stdout.write(f"{sum(a)}\n")


main()
