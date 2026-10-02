# The judge said no. Now what?

Work top to bottom. **Do not resubmit without changing something specific** —
every wrong submission on a problem you eventually solve costs 20 penalty
minutes.

---

## Compile Error

Almost always a missing header or a C++ version issue.
`bits/stdc++.h` works on the judge (GCC), so this is usually a typo or a
local-only construct.

---

## Wrong Answer

Check in this order — cheapest and most likely first.

1. **Did you read the output format exactly?** Re-read the output section
   verbatim. Singular vs plural, `Case #1:`, capitalisation, trailing text.
2. **Overflow.** Could any value exceed 2 × 10⁹? Make it `long long`.
   Check *intermediates*: `a*b` overflows even if the answer is small.
3. **Did you handle n = 0, n = 1, or an empty input?** Single-element and
   empty cases break more solutions than any other edge case.
4. **Off-by-one.** 0-indexed vs 1-indexed. `<` vs `<=`. Inclusive ranges.
5. **Are you resetting state between test cases?** If the input has multiple
   cases, clear every global array/vector/counter.
6. **Floating point.** Did you print with enough precision?
   `cout << fixed << setprecision(10)`. Never compare doubles with `==`;
   use `fabs(a-b) < 1e-9`. If the problem is exact, avoid doubles entirely.
7. **Did you misread the problem?** Re-read the statement from the top.
   At beginner level this is the single most common cause. Have your
   teammate read it independently and state the task back to you.
8. **Ties / multiple valid answers.** Does the problem say "any valid answer"?
   If not, there is a specific tie-break rule you may have missed.
9. **Run the sample by hand.** Not with your code — on paper. If your manual
   answer differs from the expected output, you misread the problem.

## Time Limit Exceeded

1. **`ios::sync_with_stdio(false); cin.tie(nullptr);`** — present? Up to 10×.
2. **`endl` → `'\n'`.** `endl` flushes; in a loop it alone causes TLE.
3. **Recompute your complexity against the real constraints.** See
   `complexity-budget.md`. Did you assume n ≤ 1000 when it is 10⁵?
4. **Accidental O(n²):** `v.erase(v.begin())` in a loop, `string +=` building
   a huge string, passing containers **by value** instead of `const &`.
5. **`unordered_map` worst case.** Under anti-hash tests it degrades to O(n).
   Swap to `map`, or sort a `vector<pair<>>`.
6. **Infinite loop.** A `while` whose condition never changes; a graph cycle
   without a visited check.

## Run Time Error

1. **Array/vector out of bounds** — by far the most common. Rebuild with
   `make d F=yourfile` (AddressSanitizer + `_GLIBCXX_DEBUG`); it names the
   exact line.
2. **Stack overflow from deep recursion.** ~10⁵ frames is the practical limit.
   Rewrite iteratively.
3. **Division or modulo by zero.**
4. **`.front()`, `.top()`, `.back()` on an empty container.**
5. **Reading past end of input** — check whether `cin >>` actually succeeded.

## Memory Limit Exceeded

1. A 2D array sized by the *maximum* constraint rather than the actual input.
   `int dp[1000][1000]` is 4 MB; `[10000][10000]` is 400 MB.
2. Declare large arrays globally, not on the stack.
3. Do you need all of the DP table, or only the previous row?

---

## Before every submission — 15-second ritual

- [ ] Compiled with **zero warnings** (`-Wall -Wextra`)
- [ ] Passes **every** provided sample, byte for byte (`make t F=...`)
- [ ] Tested n = 1 and the smallest legal input
- [ ] Every value that could exceed 2e9 is `long long`
- [ ] No leftover debug printing to stdout
- [ ] `'\n'`, not `endl`

## When to walk away

If a problem has eaten **45 minutes with no accepted submission**, switch to a
different one and come back. The scoreboard is the better guide to what is
solvable than your own attachment to the problem you already started.
