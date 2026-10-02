# NCPC 2026 — Team Reference

A **tested** C++/Python template library, a contest workflow, and a
print-ready team reference, built for the
[Nordic Collegiate Programming Contest](https://nordic.icpc.io/ncpc2026/)
(NCPC 2026, Saturday 3 October 2026, Kattis judge).

One constraint shapes this whole repository:

> **NCPC's rules changed on 2026-09-28.** Digital prewritten code is now
> banned — no local code libraries, no old solutions, no textbook PDFs on the
> contest machine. *Printed* material is allowed without limit.

So this is not a library you open during a contest. It is a library you
**compile into paper** beforehand, plus a workflow that matches the judge
exactly so nothing surprises you on the day.

---

## Start here

For the day-to-day loop, read **[START-HERE.md](START-HERE.md)** — three files:
`sol.cpp`, `in.txt`, `./run`.

## Setup

```bash
make check-toolchain     # verify your compiler matches the judge
make selftest            # run every template's built-in tests
make refs                # download third-party references (gitignored)
make print               # build/team-reference.pdf, then print it
```

## Solving a problem

Solutions live in the repo root, one file per problem: `a.cpp` … `k.cpp`.
All scratch files are gitignored.

```bash
make new P=a       # create a.cpp from the template
make run P=a       # build + run (a.in -> a.out if a.in exists)
make test P=a      # diff against a.1.in/a.1.ans, a.2.in/a.2.ans, ...
make debug P=a     # bounds-checked build; names the bad index
make brut P=a      # scaffold a_brut.cpp + gen.py
make stress P=a    # random tests until the brute force disagrees
make submit P=a    # judge-flag build + sample check before uploading
```

Full details in [`checklists/workflow.md`](checklists/workflow.md).

### Stress testing

When a solution passes the samples but the judge says Wrong Answer, stop
reading the code and let the machine find a counterexample:

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

This is the standard olympiad loop — generator → brute force → solution →
`diff -b` — as taught by [T-Olimpians](https://t-olimpians.com/wkuwanie).

## Why the toolchain matters

Kattis compiles C++ with **`g++-15` 15.2.0**, `-g -O2 -std=gnu++23 -static`,
and runs "Python 3" as **PyPy3 7.3.23 (Python 3.11.15)**.

Three traps this repo exists to defuse:

1. **On macOS, `g++` is Apple clang in disguise, and `#include <bits/stdc++.h>`
   fails.** The Makefile targets Homebrew's real `g++-15` — which happens to be
   the exact version the judge runs.
2. **The widely copied `dbg_out` macro writes to `cout`.** That injects debug
   text into the answer the judge reads: a silent Wrong Answer that looks like
   an algorithmic bug. Here `dbg()` writes to `cerr` and is wrapped in
   `#ifdef LOCAL`, so it compiles to nothing on the judge build.
3. **Your local Python is newer than 3.11.** Syntax that works locally can be
   rejected by the judge.

`make check-toolchain` prints local and expected versions side by side.

## What's here

| Path | Contents |
|---|---|
| `templates/cpp/` | Tested C++ templates; every file has a `#ifdef TEST_*` self-test |
| `templates/python/` | PyPy-aware Python skeleton with fast I/O |
| `checklists/` | Complexity budget, debugging, workflow, contest-day timeline |
| `reference/INDEX.md` | What to print, in priority order, and where it came from |
| `reference/pl/` | Polish-language material, incl. two domains to avoid |
| `practice/` | Night-before plan using NCPC 2025's easiest problems |
| `scripts/` | Reference fetcher, print-PDF generator, test generator |
| `books/` | **Gitignored.** Personally licensed PDFs never enter this repo |

### Templates

Chosen to complement [KACTL](https://github.com/kth-competitive-programming/kactl)
and to match what NCPC actually asks. KACTL's README says it excludes
"algorithms that are very common/simple (e.g., Dijkstra)" — and it ships no
Dijkstra, no plain Union-Find, no BFS/DFS. Those are exactly what NCPC's
*solvable* problems need.

| Area | Files |
|---|---|
| Data structures | `ds/dsu.cpp`, `ds/fenwick.cpp`, `ds/segment_tree.cpp` |
| Graphs | `graphs/traversal.cpp`, `graphs/dijkstra.cpp`, `graphs/algorithms.cpp` |
| DP | `dp/classic.cpp` — knapsack, coin change, LIS, LCS, edit distance, Kadane |
| Maths | `math/number_theory.cpp` — gcd/lcm, modpow, sieve, extended Euclid |
| Strings | `strings/strings.cpp` — KMP, Z-function, rolling hash |
| Geometry | `geometry/geometry.cpp` — hull, shoelace, point-in-polygon |
| Search | `binary_search.cpp`, `prefix_sums.cpp` |

Selection follows the measured 2021–2025 NCPC topic frequencies: graphs lead
at 14 of 57 problems, DP 10, geometry 9. Flows appeared **once in five years**
and suffix automata never, so they are deliberately absent.

Every self-test includes an **overflow guard** — a case large enough that an
`int` implementation would wrap — because integer overflow is the most common
single cause of wrong answers at beginner level.

```bash
$ make selftest
  binary_search                  binary search ok
  classic                        dp ok
  dsu                            DSU ok
  fenwick                        fenwick ok
  segment_tree                   segment tree ok
  geometry                       geometry ok
  algorithms                     graphs2 ok
  dijkstra                       Dijkstra ok
  traversal                      traversal ok
  number_theory                  number theory ok
  prefix_sums                    prefix ok
  strings                        strings ok
```

## Contest facts worth internalising

Derived from the NCPC 2021–2025 archives (see [`SOURCES.md`](SOURCES.md)):

- ~11 problems, 5 hours. **The problems are not sorted by difficulty** —
  stated verbatim in every year's booklet.
- The median team solves **3–5 of 11**. 15–25% of teams solve 0 or 1.
- Every year has 1–2 problems solved by **zero or one** team out of 230+.
- Each year has 4–5 problems at a 50–90% solve rate, consistently ad-hoc,
  greedy, elementary number theory, or basic graph work.
- The live scoreboard is a free difficulty oracle: with 230+ teams, the easy
  problems light up within 15–20 minutes.

## For a teammate cloning this

```bash
git clone https://github.com/DataAthleteChamp/ncpc-2026-reference
cd ncpc-2026-reference
make check-toolchain      # expect g++-15 15.2.0
./run                     # should print 6
```

Everything needed to work is already in the clone, including the printable
reference at [`print/team-reference.pdf`](print/team-reference.pdf) — no
LaTeX install required to read or print it.

Two things are **deliberately not in the repo**, and both are one command or
one rule away:

| Not committed | Why | How to get it |
|---|---|---|
| `reference/**` — KACTL, CSES, OI books, T-Olimpians | Third-party, all-rights-reserved or unclear licence. Redistributing them would be infringement. | `make refs` |
| `books/` | Personally purchased textbooks. | Keep your own copies locally. |

Sharing a solution with the team? Put it in
[`solutions/`](solutions/README.md), which is tracked. The scratch files in
the repo root (`a.cpp` … `k.cpp`) stay local on purpose: during a contest you
are on one shared machine, so they never need to travel, and keeping them
untracked avoids merge conflicts mid-contest.

If you have macOS without Homebrew GCC:

```bash
brew install gcc          # provides g++-15, the exact judge compiler
```

## Licensing and attribution

MIT-licensed. All code and prose here is original work.

**No third-party PDF, book, or code file is committed.** `make refs` downloads
freely available references onto your own machine, into a gitignored
directory. Purchased books live in `books/`, also gitignored.

See [`SOURCES.md`](SOURCES.md) for every reference consulted, its author, its
licence, and whether it may be redistributed.

## Before the contest

```bash
make print          # produce the paper
rm -rf reference/   # then remove the PDFs from the machine
```

Printed is allowed. Digital is not.
