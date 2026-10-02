# English-language references

These are **not committed**. Run `make refs` and they land here, on your own
machine. See [`../../SOURCES.md`](../../SOURCES.md) for why.

```bash
make refs
```

| File | Pages | Source | Licence |
|---|---|---|---|
| `kactl.pdf` | 26 | [KTH, kth-competitive-programming/kactl](https://github.com/kth-competitive-programming/kactl) | Mixed, mostly CC0; no repo-wide LICENSE |
| `cses-handbook.pdf` | 296 | [Antti Laaksonen, cses.fi](https://cses.fi/book/book.pdf) | Freely distributed, no formal licence |
| `stanford-notebook.pdf` | 20 | [jaehyunp/stanfordacm](https://github.com/jaehyunp/stanfordacm) | MIT |
| `suprdewd-comprog.pdf` | 27 | [Bjarki Ágúst Guðmundsson, Reykjavík U](https://github.com/SuprDewd/CompetitiveProgramming) | MIT |

## What to actually print

**KACTL** is the one to print in full. It is the standard ICPC notebook, it
comes from KTH, and it is written for exactly this contest family.

Be aware of a deliberate gap: KACTL's README states it excludes "algorithms
that are very common/simple (e.g., Dijkstra)". It ships **no Dijkstra, no
plain Union-Find** (`UnionFind.h` is commented out of its build) **and no
BFS/DFS** — those are only named in its Techniques appendix. It assumes you
know them cold.

That gap is why [`../../templates/cpp/`](../../templates/cpp) exists. Print
`print/team-reference.pdf` alongside KACTL and the gap is covered.

**CSES Handbook** is a textbook, not a flip-reference — 296 pages is too many
to navigate under time pressure. Read it tonight; print selected chapters at
most. Its author, Antti Laaksonen, runs the University of Helsinki NCPC site.

Stanford's and Reykjavík's notebooks are the same advanced tier as KACTL and
largely redundant with it. Print them only as extra insurance; NCPC sets no
page limit.

## Before the contest

```bash
rm -rf reference/
```

Printed material is unlimited. Digital prewritten code and textbook PDFs on
the machine are banned as of the 2026-09-28 rule change.
