# Shared solutions

Solved something you want the team to keep? Put it here. Everything in this
directory is **tracked and pushed**, unlike the scratch files in the repo root.

```
solutions/
  ncpc25/          practice on last year's set
    a-arithmetic-adaptation.cpp
  kattis/          individual problems
    hello.cpp
```

Suggested naming: `<kattis-problem-id>.cpp`, so anyone can find the problem
again at `https://open.kattis.com/problems/<id>`.

A one-line header comment showing the problem URL and the approach saves a
lot of re-reading later:

```cpp
// https://open.kattis.com/problems/kmh — running maximum, O(n)
```

## Why the repo root is not tracked

During the contest you work on a single shared machine (ICPC rules allow one
computer per team), so `a.cpp` … `k.cpp` never need to travel between
laptops. Keeping them untracked means the repo stays clean between contests
and you never get a merge conflict on scratch files mid-contest.

Practice is different — that is what this directory is for.
