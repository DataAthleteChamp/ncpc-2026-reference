# Complexity budget — pick the algorithm before you write it

Kattis time limits are typically **1–4 s**. Assume roughly **10⁸ simple
operations per second** for C++ at `-O2`. Read `n` from the constraints
section, find the row, and that tells you what you are allowed to write.

| max n | Budget | What fits | Typical technique |
|---|---|---|---|
| ≤ 10 | O(n!) | 3.6M | brute-force all permutations |
| ≤ 20 | O(2ⁿ · n) | 20M | bitmask / subset enumeration |
| ≤ 100 | O(n³) | 1M | Floyd–Warshall, triple loop, small DP |
| ≤ 1 000 | O(n²) | 1M | pairwise loops, 2D DP, O(n²) graph |
| ≤ 10⁵ | O(n log n) | 1.7M | **sort, set/map, binary search, Dijkstra** |
| ≤ 10⁶ | O(n) | 1M | prefix sums, two pointers, single sweep |
| ≤ 10⁹ | O(log n) / O(√n) | — | binary search on answer, math, trial division |
| ≥ 10¹⁸ | O(1) | — | closed-form formula only |

**The single most useful inference:** `n ≤ 10⁵` almost always means
`O(n log n)` — i.e. *sort it, or put it in a set/map, or binary search it*.
`n ≤ 10⁶` means you get one linear pass and nothing more.

## Reading the constraint backwards

If you cannot see the intended algorithm, the constraint tells you the shape:

- `n ≤ 20` → the answer is a subset or permutation search. Stop looking for a clever polynomial algorithm.
- `n ≤ 5000` with a 2D feel → O(n²) DP.
- Sum of n over all test cases bounded → your per-case cost must be near-linear.
- Answer is "minimum x such that..." / "maximum x such that..." → **binary search on the answer**; you only need a feasibility check.
- Huge coordinate range but few points → coordinate compression.

## Constant-factor reality checks

These matter more than beginners expect:

| Thing | Cost |
|---|---|
| `cin`/`cout` without `sync_with_stdio(false)` | up to **10× slower** — always disable |
| `endl` instead of `'\n'` | flushes every line; can alone cause TLE |
| `map` / `set` | ~5–10× slower than `unordered_*` and than sorting a `vector` |
| `unordered_map` | O(1) average but **O(n) worst case** under anti-hash tests |
| recursion | ~2–3× a loop, and stack-overflows near 1e5 depth |
| `%` and `/` on `long long` | several times the cost of `int` |

## Overflow budget

| Type | Max |
|---|---|
| `int` | ≈ 2.1 × 10⁹ |
| `long long` | ≈ 9.2 × 10¹⁸ |
| `double` | exact integers only to 2⁵³ ≈ 9 × 10¹⁵ |

**Rule:** if any intermediate value can exceed ~2 × 10⁹, use `long long`
everywhere in that expression. `n * (n-1) / 2` overflows `int` at
n ≈ 65 536 — far below a typical `n ≤ 10⁵` bound. This is the most common
single cause of wrong answers at beginner level.

Note `1 << 31` overflows; write `1LL << 31`.
