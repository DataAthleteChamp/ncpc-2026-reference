# Sources and attribution

This repository is MIT-licensed (see [`LICENSE`](LICENSE)). **All code under
`templates/` and all prose under `checklists/` is original work written for
this repository**, informed by the references below but not copied from them.

We take open-source attribution seriously, and we also take *not*
redistributing other people's work seriously. The policy is:

> **We link. We do not vendor.**
> No third-party PDF, book, or code file is committed to this repository.
> `scripts/fetch-references.sh` downloads freely available references to a
> gitignored directory on your own machine.

---

## Why no PDFs are committed

Three separate reasons, any one of which is sufficient:

1. **Licensing.** Several of the best references have unclear or
   non-redistributable licensing (KACTL has no repo-wide `LICENSE` file;
   the CSES handbook states no formal license; T-Olimpians material is
   proprietary).
2. **Copyright.** Purchased or personally licensed books must never be
   republished. They live in the gitignored `books/` directory.
3. **Contest rules.** NCPC 2026 bans digital prewritten code on the contest
   machine. A repository full of PDFs encourages exactly the behaviour the
   rules forbid. See below.

---

## References consulted

### Contest rules and data

| Source | URL | Used for |
|---|---|---|
| NCPC 2026 rules | <https://nordic.icpc.io/ncpc2026/> | Rule change of 2026-09-28; printed-material policy |
| NCPC rules source | <https://github.com/icpc/ncpc-web> | Change-log verification |
| Kattis language settings | <https://open.kattis.com/languages/cpp> | Exact judge compiler: `g++-15 15.2.0`, `-g -O2 -std=gnu++23 -static` |
| Kattis Python settings | <https://open.kattis.com/languages/python3> | Judge runs PyPy3 7.3.23 (Python 3.11.15) |
| NCPC 2021–2025 archives | `ncpc21..ncpc25.kattis.com` | Problem counts, difficulty, solve rates |
| NCPC official solution slides | Codeforces Gym `104670`, `104030`, `105427`, `105431`, `106124` | Topic classification of 57 problems |

The topic-frequency and solve-rate figures quoted in `checklists/` are derived
from the five archives above, not from memory.

### Algorithm references

| Source | Author | URL | License |
|---|---|---|---|
| **KACTL** | Simon Lindholm, Johan Sannemo, Mårten Wiman (KTH) | <https://github.com/kth-competitive-programming/kactl> | Mixed; mostly CC0, no repo-wide license |
| **Competitive Programmer's Handbook** | Antti Laaksonen | <https://cses.fi/book/book.pdf> | Freely distributed, no formal license |
| **cp-algorithms** | community | <https://cp-algorithms.com> | CC BY-SA 4.0 |
| **Stanford ACM notebook** | jaehyunp et al. | <https://github.com/jaehyunp/stanfordacm> | MIT |
| **CompetitiveProgramming** | Bjarki Ágúst Guðmundsson (Reykjavík U) | <https://github.com/SuprDewd/CompetitiveProgramming> | MIT |

**KACTL deliberately omits Dijkstra, plain Union-Find, and BFS/DFS** — its
README states it excludes "algorithms that are very common/simple". That gap
is the specific reason `templates/cpp/graphs/` and `templates/cpp/ds/dsu.cpp`
exist here.

### Polish-language references

Full detail, including two domains to avoid, is in
[`reference/pl/README.md`](reference/pl/README.md).

| Source | URL | Licence | Status |
|---|---|---|---|
| **T-Olimpians — Arkusz referencyjny: Struktury danych w C++** | <https://t-olimpians.com> | © T-Olimpians / Team Poland | Not redistributable — personal copy only |
| **`wzorki.md`** (UWr, Algorytmika Praktyczna) | <https://github.com/PatrykFlama/UWr> | No `LICENSE` file → all rights reserved | Link only |
| **W poszukiwaniu wyzwań 2** | <https://www.mimuw.edu.pl/~idziaszek/algonotes/looking-for-a-challenge-2-pl.pdf> | © Wydział MIM UW, no open licence | Link only; free and legal to download |
| Olimpiada Informatyczna "niebieskie książeczki" (23 vols) | <https://oi.edu.pl/l/oi_zadania/> | © Komitet Główny OI | Link only |
| Szkopuł (judge, SIO2) | <https://szkopul.edu.pl> | © | Link only; hosts no tutorials of its own |
| Polska Wikipedia | <https://pl.wikipedia.org> | **CC BY-SA** | **Redistributable** with attribution |

`templates/cpp/math/number_theory.cpp` gained `extgcd` and `modinvGeneral`
after reading `wzorki.md`: our original `modinv` used Fermat's little theorem
and therefore only worked for prime moduli.

**Two domains deliberately not linked anywhere in this repo:**

- `main.edu.pl` — the domain was squatted and now serves an online-casino
  affiliate page ("Młodzieżowa Akademia Internetowych Kasyn"). It still
  appears in older Polish CP guides. Its successor is Szkopuł.
- `olimpiada.edu.pl` — this is *Olimpiada Przedsiębiorczości*, the
  Entrepreneurship Olympiad. The informatics olympiad is `oi.edu.pl`.

---

## Contributing

If you adapt an algorithm from a third-party source, say so in a comment on
the function, with the source and its license. If the license does not permit
redistribution, write your own implementation instead of copying.
