# Development workflow

One command per thing you actually do during a contest. Solutions live in the
repo root, one file per problem: `a.cpp`, `b.cpp`, … `k.cpp`.

```bash
make new P=a          # create a.cpp from the template
make run P=a          # build + run (a.in -> a.out if a.in exists)
make test P=a         # diff against a.1.in/a.1.ans, a.2.in/a.2.ans, ...
make debug P=a        # bounds-checked build; finds out-of-bounds
make brut P=a         # scaffold a_brut.cpp + gen.py
make stress P=a       # random tests until brute force disagrees
make submit P=a       # judge-flag build + sample check before uploading
make clean P=a
```

All scratch files (`a.cpp`, `a.*.in`, `gen.py`, `in.txt`, binaries) are
gitignored, so the repo stays clean between contests.

## Typical loop on one problem

```bash
make new P=a                 # start
pbpaste > a.1.in             # paste the sample input from the problem page
#   ...write the expected output into a.1.ans...
make test P=a                # must pass before you even think about submitting
make submit P=a              # rebuilds with judge flags, re-runs samples
```

`make test` compares byte for byte. If it prints FAIL with identical-looking
strings, you have a trailing space or newline difference.

## Three ways to feed input

All three work; use whichever is fastest in the moment.

| How | When |
|---|---|
| `make run P=a` with `a.in` present | the normal case |
| `./a < a.in > a.out` | manual control |
| put data in `in.txt`, just run `./a` | pressing Run in an editor, no shell |

The `in.txt` path works because `solution.cpp` calls `freopen` — but only
inside `#ifdef LOCAL`, and only if `in.txt` actually exists. The judge build
never defines `LOCAL`, so a stray `freopen` cannot reach Kattis. This matters:
an unguarded `freopen` in a submission makes the judge read a file that does
not exist, and every test fails for a reason that looks nothing like the cause.

## Debug output

`dbg(...)` prints to **stderr**, which Kattis ignores, and compiles to nothing
without `-DLOCAL`:

```cpp
dbg(n, a);     // [90] (n, a): 3 {1, 2, 3}
```

It formats vectors, pairs, sets and maps automatically.

> **Why this matters.** The widely copied `dbg_out` macro writes to `cout`.
> That injects debug text into the answer the judge reads — a silent Wrong
> Answer that looks like an algorithmic bug. You will not notice locally,
> because locally you are reading your own output.

## Stress testing — finding the bug you cannot see

When a solution passes the samples but gets Wrong Answer, and you cannot see
why, stop reading the code and let the computer find a counterexample. This is
the standard olympiad technique:

```
gen.py SEED  ->  random small test
a_brut.cpp   ->  slow solution that is OBVIOUSLY correct
a.cpp        ->  your fast solution
diff -b      ->  stop at the first disagreement
```

```bash
make brut P=a      # creates a_brut.cpp and gen.py
#   ...write the O(n^2) or O(n!) version in a_brut.cpp...
#   ...edit gen.py so it emits this problem's input format...
make stress P=a    # or: make stress P=a N=1000
```

Output on failure:

```
MISMATCH on seed 1  (reproduce: python3 gen.py 1)
--- input (stress.in) ---
3
10 2 5
--- yours ---
12
--- brute force ---
17
```

Keep the generator **small** — `n ≤ 8`, values `≤ 10`. A counterexample you
can read in three seconds is worth far more than a correct-but-huge one, and
the brute force has to finish quickly.

## Sanitizers, honestly

`make debug` uses `_GLIBCXX_DEBUG`, not AddressSanitizer. On macOS:

- Homebrew GCC ships **no** `libasan` or `libubsan`, so `-fsanitize=` cannot link.
- Apple clang has the sanitizers but has **no** `<bits/stdc++.h>` and no
  `__gnu_pbds`, so it cannot compile the template at all.

`_GLIBCXX_DEBUG` catches the dominant contest bug — container indexing — and
reports it precisely:

```
Error: attempt to subscript container with out-of-bounds index 9,
but container only holds 3 elements.
```

## Python

```bash
python3 sol.py < a.in > a.out     # normal
LOCAL=1 python3 sol.py            # reads in.txt, writes out.txt, dbg() on
```

Remember Kattis's "Python 3" is **PyPy 7.3.23 (Python 3.11.15)**. Loops are
fast; `numpy` is not available. Do not use 3.12+ syntax.
