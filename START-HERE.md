# START HERE

> **Saturday:** print the reference, then run `make contest-ready` to remove
> this repo from the laptop. Prewritten code is legal **on paper only** —
> see [`checklists/contest-legal.md`](checklists/contest-legal.md).

Three files in this folder are your workspace:

```
sol.cpp     <- write your solution here
in.txt      <- paste the problem's sample input here
run         <- run it
```

That is the whole thing.

## The loop

1. Open **`sol.cpp`**. Write your code inside `solve()`.
2. Open **`in.txt`**. Paste the sample input from the problem page.
3. In the terminal:

```bash
./run
```

You get:

```
--- input (in.txt) ---
3
1 2 3
--- debug (stderr) ---
[90] (n, a): 3 {1, 2, 3}
--- answer (stdout -> out.txt) ---
6
```

Compare the **answer** block against the problem's expected output. If it
matches, submit `sol.cpp` to Kattis.

## Printing debug values

```cpp
dbg(n, a);          // [90] (n, a): 3 {1, 2, 3}
dbg(x, y, found);   // works with any number of values
```

It prints vectors, pairs, sets and maps automatically.

**`dbg` is safe to leave in your submission.** It writes to stderr (which the
judge ignores) and compiles to nothing without `-DLOCAL`, which the judge
build never sets. This is why you must never `cout` your debug output — that
goes straight into the answer and causes a Wrong Answer that looks like an
algorithm bug.

## More than one problem at a time

During the contest you will have several open. Use one letter per problem:

```bash
cp sol.cpp b.cpp      # start problem B
./run b               # builds b.cpp, reads b.in
```

Or with the Makefile, which does the same thing plus sample checking:

```bash
make new P=b          # create b.cpp
make run P=b          # run it
make test P=b         # check against b.1.in / b.1.ans
make submit P=b       # rebuild exactly as the judge does, re-check samples
```

## When you are stuck

| Problem | Command |
|---|---|
| Crash, or suspect out-of-bounds | `make debug P=sol` |
| Passes samples, judge says Wrong Answer | `make stress P=sol` |
| Want to check several samples at once | `make test P=sol` |
| Not sure your compiler matches the judge | `make check-toolchain` |

`make stress` generates random small tests and compares your solution against
a slow one you trust, stopping at the first disagreement. See
[`checklists/workflow.md`](checklists/workflow.md).

## Reference material

- [`checklists/complexity-budget.md`](checklists/complexity-budget.md) — which
  algorithm fits the input size
- [`checklists/debug.md`](checklists/debug.md) — what to check on WA / TLE / RTE
- [`checklists/contest-day.md`](checklists/contest-day.md) — timings, venue, plan
- [`templates/cpp/`](templates/cpp) — DSU, Fenwick, segment tree, Dijkstra, DP,
  strings, geometry. All self-tested: `make selftest`
