# Contest day — NCPC 2026

**Saturday 3 October 2026 · doors 10:00 · contest 11:00–16:00 CEST**
**IT-Universitetet i København**, Rued Langgaards Vej 7, 2300 København S
(site for ITU + DTU + KU). Food during, pizza and beer after.

Also runs as **DM i programmering** — the Danish championship.

---

## The night before

- [ ] Confirm the team is registered and you have **Kattis team credentials**
      (a team account, *not* your personal Kattis login). The contest lives at
      `ncpc26.kattis.com`.
- [ ] Print everything. See `reference/INDEX.md`.
- [ ] **Delete the reference PDFs from the laptop.** Printed = allowed;
      the digital file on disk is exactly what the 2026-09-28 rule change bans.
- [ ] Disable any AI/LLM editor plugin (Copilot, Codeium, Cursor tab). These
      are explicitly disallowed. Turn them off at the settings level, not just
      visually.
- [ ] `make check-toolchain` — confirm `g++-15` is what builds your code.
- [ ] Charger, mouse, laptop. One computer, one keyboard, one mouse, one screen.
- [ ] Sleep. Genuinely higher value than another hour of cramming.

## What you may bring

| Allowed | Banned |
|---|---|
| Unlimited printed material | Any digital prewritten code |
| Textbooks, team reference docs | PDFs of textbooks on the machine |
| One laptop, one keyboard/mouse/screen | Phones and any extra device |
| `cppreference.com`, `docs.python.org` (built-in search only) | Any other website |
| Basic editor autocomplete | Any GenAI/LLM tool or plugin |

---

## 10:00 — doors open

Arrive at 10:00, not 10:55. Set up, log in, open the scoreboard and the
submit page, and leave them open. Put the printed references where both of
you can reach them.

## 11:00–11:20 — read everything, write nothing

The problems are **not sorted by difficulty** — this is stated verbatim in
every NCPC booklet. Letter order tells you nothing.

With two people:

- Person A reads A → K. Person B reads K → A.
- Score each problem on a sheet: **E** (I see the solution now),
  **M** (I see the shape, needs thought), **H** (no idea).
- Do **not** start coding during this window, even if problem C looks trivial.

## 11:20 — first commitment

Start the problem you both scored **E** with the shortest implementation.
Not the most interesting one. Shortest.

## 11:30 onwards — use the scoreboard as a difficulty oracle

With 230+ teams competing, the easy problems light up within 15–20 minutes.
**Check the scoreboard every ~20 minutes.** If 40 teams have solved problem
G and you had it as H, re-read G — you misjudged it. This signal is far more
reliable than a beginner's own read of a statement.

## Role split for two people

One keyboard means the bottleneck is thinking, not typing.

- **Coder** writes the current solution.
- **Reader** does *not* watch the screen. The reader works the next problem on
  paper: edge cases, approach, the complexity check from
  `complexity-budget.md`. Hand over a written plan, then swap roles.
- When a submission comes back WA, the **reader** runs `debug.md` out loud
  while the coder re-reads the statement. Two people, two different inputs.

## Pacing

| Time | Target | If you are behind |
|---|---|---|
| 12:00 | 1 solved | Drop to the problem with the most scoreboard solves |
| 13:00 | 2–3 solved | Stop any problem older than 45 min |
| 14:00 | 3–4 solved | Pick purely by scoreboard count now |
| 15:00 | 4–5 solved | Finish what is nearly done; start nothing new |
| 15:45 | — | Last realistic submit window |

## Penalty maths — worth understanding

Penalty = (minutes to first accepted) + 20 × (wrong submissions **on solved
problems**). Wrong submissions on problems you never solve are **free**.

Two consequences:

1. Never leave a problem unsubmitted at the end out of fear. A speculative
   submission at 15:55 costs nothing if it fails.
2. During the contest, solving one more problem always beats optimising
   penalty. Rank is by count first; penalty only breaks ties.

## Realistic expectations

Across 2021–2025, the median NCPC team solved **3–5 of 11**, and 15–25% of
all teams solved 0 or 1. Every year has 1–2 problems solved by **zero or one**
team out of 230+. You are not expected to solve the hard half. Solve the easy
half cleanly and you finish mid-table or better.

## If something breaks

Submit a **clarification request** through the judge system. Expect
"No comment, read problem statement" — that answer means your reading is
correct and there is no ambiguity, which is itself useful information.
