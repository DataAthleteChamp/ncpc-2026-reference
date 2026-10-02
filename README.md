# NCPC 2026 — Team Reference

A small, **tested** C++/Python template library and printable team reference,
built for the [Nordic Collegiate Programming Contest](https://nordic.icpc.io/ncpc2026/)
(NCPC 2026, Saturday 3 October 2026, Kattis judge).

Designed around one constraint that makes this repo different from most
competitive-programming notebooks:

> **NCPC's rules changed on 2026-09-28.** Digital prewritten code is now
> banned — no local code libraries, no old solutions, no textbook PDFs on the
> contest machine. *Printed* material is allowed without limit.

So this repo is not meant to be open during a contest. It is meant to be
**compiled into paper** beforehand, and to give you a build/test workflow that
matches the judge exactly.

---

## Quick start

```bash
make check-toolchain     # verify your compiler matches the judge
make refs                # download third-party references (gitignored)
make print               # render build/team-reference.pdf, then print it
```

Day-to-day, while solving:

```bash
make r F=a               # build a.cpp with judge flags, then run it
make t F=a               # run a.cpp against a.1.in/a.1.ans, a.2.in/... and diff
make d F=a               # sanitized build: finds the out-of-bounds for you
make p F=templates/cpp/ds/dsu    # run a template's own self-test
```

## Why the toolchain matters

Kattis compiles C++ with **`g++-15` 15.2.0**, `-g -O2 -std=gnu++23 -static`,
and runs "Python 3" as **PyPy3 7.3.23 (Python 3.11.15)**.

Two traps this repo exists to defuse:

1. **On macOS, `g++` is Apple clang in disguise, and `#include <bits/stdc++.h>`
   fails.** The `Makefile` points at Homebrew's real `g++-15`, which happens to
   be the exact version the judge runs.
2. **Your local Python is probably newer than 3.11.** Syntax that works on your
   machine can be rejected by the judge.

`make check-toolchain` prints both local and expected versions side by side.

## What's here

| Path | Contents |
|---|---|
| `templates/cpp/` | Tested C++ templates. Every file has a `#ifdef TEST_*` self-test. |
| `templates/python/` | PyPy-aware Python skeleton with fast I/O. |
| `checklists/` | Complexity budget, WA/TLE/RTE debugging, contest-day timeline. |
| `reference/INDEX.md` | What to print, in priority order, and where it comes from. |
| `scripts/` | Reference fetcher and the print-ready PDF generator. |
| `books/` | **Gitignored.** Personally licensed PDFs never enter this repo. |

### Templates

Chosen deliberately to complement [KACTL](https://github.com/kth-competitive-programming/kactl)
rather than duplicate it. KACTL's README states it excludes "algorithms that
are very common/simple (e.g., Dijkstra)" — and indeed it ships no Dijkstra, no
plain Union-Find, and no BFS/DFS. Those are exactly the algorithms NCPC's
*solvable* problems need, so they are the core of this library.

| File | Why |
|---|---|
| `ds/dsu.cpp` | Union-Find. KACTL comments it out of the build. |
| `graphs/dijkstra.cpp` | KACTL omits it by design. |
| `graphs/traversal.cpp` | BFS/DFS/grid BFS. KACTL only names these in an appendix. |
| `binary_search.cpp` | Binary search on the answer — highest-value beginner technique. |
| `prefix_sums.cpp` | 1D/2D prefix sums and difference arrays. |
| `math/number_theory.cpp` | gcd/lcm, modpow, sieve, factorisation. |

Every template's self-test includes an **overflow guard** — a case large enough
that an `int` implementation would wrap — because integer overflow is the most
common single cause of wrong answers at beginner level.

```bash
$ make p F=templates/cpp/math/number_theory
number theory ok
```

### Checklists

- **`complexity-budget.md`** — map `n` to the complexity you are allowed, plus
  constant-factor costs and the overflow table. Read the constraints, find the
  row, pick the algorithm *before* writing code.
- **`debug.md`** — ordered WA / TLE / RTE / MLE triage, and a pre-submit
  ritual. Wrong submissions cost 20 penalty minutes each on solved problems.
- **`contest-day.md`** — venue and timings, what you may bring, the
  minute-by-minute plan, and the two-person role split.

## Contest facts worth internalising

Derived from NCPC 2021–2025 archives (see [`SOURCES.md`](SOURCES.md)):

- ~11 problems, 5 hours. **The problems are not sorted by difficulty** — stated
  verbatim in every year's booklet.
- The median team solves **3–5 of 11**. 15–25% of teams solve 0 or 1.
- Every year has 1–2 problems solved by **zero or one** team out of 230+.
- Each year has 4–5 problems at a 50–90% solve rate, and they are consistently
  ad-hoc, greedy, elementary number theory, or basic graph work.
- Flows appeared **once in five years**. Suffix arrays, once. Do not train for
  them the night before.
- The live scoreboard is a free difficulty oracle: with 230+ teams, the easy
  problems light up within 15–20 minutes.

## Licensing and attribution

This repository is MIT-licensed. All code and prose here is original work.

**No third-party PDF, book, or code file is committed.** `make refs` downloads
freely available references onto your own machine, into a gitignored
directory. Purchased or personally licensed books live in `books/`, which is
also gitignored and never published.

See [`SOURCES.md`](SOURCES.md) for every reference consulted, its author, its
licence, and whether it may be redistributed.

## Before the contest

```bash
make print          # produce the paper
rm -rf reference/   # then remove the PDFs from the machine
```

Printed is allowed. Digital is not.
