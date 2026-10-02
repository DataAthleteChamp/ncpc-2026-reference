# Practice plan — the night before

The NCPC organisers' own advice is to solve last year's set:

> "If you're new to Kattis, we encourage you to familiarize yourself with the
> system by trying it out before the contest. We suggest that you check out the
> problems from last year's NCPC."
> — <https://nordic.icpc.io/ncpc2026/compete>

There is **no official practice contest**. Do this instead.

## Priority 0 — submit something tonight (30 min)

Before any algorithm practice, prove the whole pipeline works:

1. Log in to Kattis. Confirm you have **team credentials** for
   `ncpc26.kattis.com` — a team account, not your personal login.
2. Solve and submit <https://open.kattis.com/problems/hello> (prints one line).
3. Confirm you see a green **Accepted**.

Discovering a broken submission flow at 11:05 tomorrow costs you the contest.
This is the single highest-value 30 minutes available tonight.

## Priority 1 — NCPC 2025's easy five (2.5 h)

All verified live. Ordered by how many teams solved them at NCPC 2025, i.e.
easiest first. These are exactly the problems your target of 4–6 depends on.

| # | Problem | Solved | Kattis difficulty | Shape |
|---|---|---|---|---|
| 1 | [Arithmetic Adaptation](https://open.kattis.com/problems/arithmeticadaptation) | 85.1% | 1.8 | ad-hoc |
| 2 | [km/h](https://open.kattis.com/problems/kmh) | 80.3% | 1.5 | running maximum |
| 3 | [Crochet Competition](https://open.kattis.com/problems/crochetcompetition) | 76.6% | 1.9 | modular time arithmetic |
| 4 | [Instagraph](https://open.kattis.com/problems/instagraph) | 60.6% | 2.3 | basic graph degrees |
| 5 | [Gotta Trade Some of 'Em](https://open.kattis.com/problems/gottatradesomeofem) | 52.8% | 3.1 | connectivity — union-find / BFS |

Note what these have in common: **no advanced data structures at all**. Four
of the five are arithmetic and careful reading. That is the real NCPC easy
tier, and it is why `checklists/debug.md` matters more than any template.

Work them under a clock. For each one:

```bash
make t F=a       # must pass every sample before you submit
```

If you finish early, the next two up are
[Dune Dash](https://open.kattis.com/problems/dunedash) (23.8%) and
[Bohemian Bookshelf](https://open.kattis.com/problems/bohemianbookshelf)
(13.0%) — these are where it gets genuinely hard.

## Priority 2 — the two-person drill (30 min)

Pick one unsolved problem and rehearse the split from
`checklists/contest-day.md`:

- One person at the keyboard, the other **not looking at the screen**, writing
  the approach and edge cases on paper.
- Hand over the written plan. Swap roles.
- When a submission fails, the non-coder reads `debug.md` aloud while the
  coder re-reads the statement.

The point is not to solve the problem. It is to make the handover feel normal
before it matters.

## Priority 3 — sleep

Genuinely higher expected value than a sixth hour of cramming. The contest is
five hours of sustained careful reading; fatigue costs more than one extra
memorised algorithm gains.

## Full archives

Every past NCPC, for later practice:

| Year | Judge | Gym mirror (virtual contest + solution slides) |
|---|---|---|
| 2025 | <https://ncpc25.kattis.com/problems> | `codeforces.com/gym/106124` |
| 2024 | <https://ncpc24.kattis.com/problems> | `codeforces.com/gym/105431` |
| 2023 | <https://ncpc23.kattis.com/problems> | `codeforces.com/gym/105427` |
| 2022 | <https://ncpc22.kattis.com/problems> | `codeforces.com/gym/104030` |
| 2021 | <https://ncpc21.kattis.com/problems> | `codeforces.com/gym/104670` |

The Gym mirrors carry the **official solution slides**, which are the best
source for understanding what the intended approach was.
