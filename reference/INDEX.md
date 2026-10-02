# Reference index

Everything the team can consult, split by language and by whether it is
redistributable. **Nothing in this index is committed as a binary** — see
[`SOURCES.md`](../SOURCES.md) for licensing rationale and
[`scripts/fetch-references.sh`](../scripts/fetch-references.sh) to download the
freely redistributable items.

> **NCPC 2026 rule:** printed material is unlimited and unrestricted; *digital*
> prewritten code and textbook PDFs are banned. Print these, then remove the
> PDFs from the contest laptop.

## Print priority

Tier 1 is what you actually open during the contest. Tier 3 is insurance you
will probably never flip to — but paper is free under NCPC rules.

| Tier | Document | Lang | Pages | Why |
|------|----------|------|-------|-----|
| 1 | T-Olimpians — *Arkusz referencyjny: Struktury danych w C++* | PL | 8 | STL + complexities + contest traps. Highest density per page available. |
| 1 | `wzorki.md` — UWr formula sheet | PL | ~5 | Extended Euclid, modular inverse, Euler totient, CRT, geometry. |
| 1 | `checklists/` (this repo) | PL/EN | ~6 | Debug checklist, complexity budget, contest timeline. Written for this team. |
| 1 | `templates/` printed (this repo) | EN | ~10 | Fills KACTL's deliberate gaps (Dijkstra, plain DSU, BFS/DFS). |
| 2 | KACTL — KTH team reference | EN | 26 | The standard ICPC notebook. Nordic origin. |
| 2 | C++ STL Containers Wall Sheet | EN | 3 | Local copy; complexity-first layout. |
| 3 | *W poszukiwaniu wyzwań 2* | PL | 212 | Polish collegiate-contest problems + solutions. Same difficulty band as NCPC. |
| 3 | CSES *Competitive Programmer's Handbook* | EN | 296 | Textbook, not a flip-reference. Print selected chapters only. |
| 3 | OI "niebieska książeczka" (e.g. `oi23.pdf`) | PL | 252 | Task editorials only, no theory chapters. Use Etap I surgically. |
| 3 | Personal library (`books/`, gitignored) | PL/EN | — | Deep insurance. Listed below by title only. |

Polish material is indexed separately in [`pl/README.md`](pl/README.md),
including two domains that now redirect to unrelated or malicious content.

## Tier 1 — English (`reference/en/`)

| Document | Source | License | Redistributable |
|---|---|---|---|
| `kactl.pdf` | <https://github.com/kth-competitive-programming/kactl> | Mixed, mostly CC0; no repo-wide LICENSE | Link only |
| `book.pdf` (CSES Handbook) | <https://cses.fi/book/book.pdf> | Freely distributed, no formal license | Link only |

## Tier 1 — Polish (`reference/pl/`)

See [`pl/README.md`](pl/README.md) for the full annotated list.

| Document | Source | License | Redistributable |
|---|---|---|---|
| `sciaga-struktury-danych-cpp-t-olimpians.pdf` | <https://t-olimpians.com> | © T-Olimpians / Team Poland | **No** — personal copy only |
| `wzorki.md` | <https://github.com/PatrykFlama/UWr> | No LICENSE file | **No** — link only |
| `w-poszukiwaniu-wyzwan-2.pdf` | <https://www.mimuw.edu.pl/~idziaszek/algonotes/> | © Wydział MIM UW | **No** — free to download, not to republish |
| `oi23.pdf` | <https://oi.edu.pl/l/oi_zadania/> | © Komitet Główny OI | **No** — link only |

## Personal library (local only, `books/`)

Indexed by title so the team knows what insurance exists. Files are gitignored.

- *Competitive Programmer's Handbook* — Antti Laaksonen (2018 draft; prefer the current build from cses.fi)
- *Algorithm Design* — Kleinberg & Tardos
- *Fundamentals of Algorithmics* — Brassard & Bratley
- *Zaprzyjaźnij się z algorytmami* — Bhargava, Polish ed. (beginner-paced; study aid, not a contest reference)
- *Cracking the Coding Interview* — McDowell (low ROI for ICPC; different problem genre)
