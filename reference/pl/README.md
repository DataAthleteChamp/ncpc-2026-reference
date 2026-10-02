# Materiały polskojęzyczne / Polish-language references

Everything below was fetched and verified directly. HTTP statuses and file
sizes are real, checked on 2026-10-02.

> **Licensing:** none of this is redistributable. Polish CP material is
> almost universally all-rights-reserved — GitHub repos without a `LICENSE`
> file included. We link; we do not vendor. See [`../../SOURCES.md`](../../SOURCES.md).

---

## ⚠️ Two domains to avoid

These appear in older Polish CP guides and are now actively misleading:

| Domain | What it actually is now |
|---|---|
| **`main.edu.pl`** | **Squatted.** Now serves "MAIN — Młodzieżowa Akademia Internetowych Kasyn", an online-casino affiliate page. The old MAIN judge is gone; its successor is Szkopuł. Do not visit, do not link. |
| **`olimpiada.edu.pl`** | **Wrong olympiad.** This is *Olimpiada Przedsiębiorczości* (the Entrepreneurship Olympiad, run by SGH). Nothing to do with informatics. The informatics olympiad is **`oi.edu.pl`** only. |

---

## Tier 1 — print these

### T-Olimpians — *Arkusz referencyjny: Struktury danych w C++*

8 pages. Every STL container with its methods, time complexities, and a
**„pułapka konkursowa"** (contest trap) note each — `vector`, `pair`, `deque`,
`queue`, `stack`, `priority_queue`, `set`, `unordered_set`, `map`,
`unordered_map`, `bitset`, plus `ordered_set` (pbds) and a summary table.

The highest information density per page of anything available to this team,
in either language. Source: <https://t-olimpians.com> (Team Poland).
**Proprietary — personal copy only, not fetched by the script.**

### `wzorki.md` — Uniwersytet Wrocławski, kurs Algorytmika Praktyczna

<https://github.com/PatrykFlama/UWr/blob/main/Sem6/AP/wzorki.md>
(verified HTTP 200, 6 171 bytes, 185 lines)

Condensed formula sheet with pseudocode:

- **Teoria liczb** — rozszerzony Euklides, odwrotność modularna, funkcja
  Eulera, chińskie twierdzenie o resztach, dwumian Newtona
- **Geometria** — iloczyn wektorowy, pole wielokąta, odległość od prostej

This maps almost exactly onto NCPC's actual number-theory profile, which is
elementary (gcd/lcm, modular arithmetic, parity) in 9 of 57 problems across
2021–2025. No `LICENSE` file, so **link only**.

> This file is why `templates/cpp/math/number_theory.cpp` gained `extgcd` and
> a general `modinvGeneral` — our original `modinv` only worked for prime
> moduli, which `wzorki.md` makes obvious is insufficient.

---

## Tier 2 — the best free Polish book

### *W poszukiwaniu wyzwań 2* — Diks, Idziaszek, Łącki, Radoszewski

<https://www.mimuw.edu.pl/~idziaszek/algonotes/looking-for-a-challenge-2-pl.pdf>

212 pages, 1 357 923 bytes, free and legal, hosted by Wydział MIM UW.
Problems and full solutions from Akademickie Mistrzostwa Polski w
Programowaniu Zespołowym 2011–2014 — **collegiate team-contest level, which
is the same difficulty band as NCPC**. The best free Polish resource found.

> **Gotcha:** the server returns **403 to a bare `curl`**. You must send a
> normal browser `User-Agent`. `scripts/fetch-references.sh` does this.

Copyright Wydział MIM UW, no open licence → personal printing fine, do not
redistribute.

---

## Tier 3 — Olimpiada Informatyczna "niebieskie książeczki"

<https://oi.edu.pl/l/oi_zadania/>

**23 compiled volumes (I–XXIII, 1993/94–2015/16), all downloadable**, each
roughly 120–260 pages. After edition XXIII the committee stopped publishing
annual books; editions XXIV–XXXIII exist only as loose per-task PDFs.

Examples (verified live):

- `https://oi.edu.pl/media/attachment/20171006/oi23.pdf` — XXIII OI, 252 pp.
- `https://oi.edu.pl/media/attachment/20140306/oi20.pdf` — XX OI, 204 pp.

**Important, and contrary to common belief: these contain no theory
chapters.** Every volume is *Sprawozdanie → Regulamin → Zasady organizacji →
opracowania zadań*. You cannot "read the DP chapter" — the technique is
taught inside individual task editorials.

**Use surgically.** 23 × ~200 pages is over 4 000 pages. Only **Etap I**
(Stage I) approaches NCPC-easy difficulty; Stages II–III escalate toward IOI
level and are low ROI for a beginner team.

Copyright Komitet Główny Olimpiady Informatycznej. Link only.

---

## Deliberately skipped, and why

High prestige, low ROI for this contest:

| Item | Why not |
|---|---|
| *Wprowadzenie do algorytmów* (CLRS PL) | 1200+ pp. Paid. Far too dense for overnight use or in-contest lookup. |
| *Matematyka konkretna* (Concrete Mathematics PL) | Graduate-level maths. Wrong tool entirely. |
| *Przygody Bajtazara* | **Commonly believed free — it is not.** Paid PWN 2018 title; only a retailer preview fragment is free. |
| *Algorytmika praktyczna* (Stańczyk) | Well-targeted, but no legal free PDF exists. |
| *Kombinatoryka dla programistów* (Lipski) | Narrow, and effectively out of print. |
| Banachowski/Diks/Rytter | Good textbook. If you own the physical copy, bringing it is legal and fine; otherwise unobtainable tonight. |

Dead ends, recorded so nobody repeats the search: AGH's open-resource
algorithms page is a 2009 stub; UJ's `tcs.uj.edu.pl` is a news blog;
`satori.tcs.uj.edu.pl` is a login-gated judge; Szkopuł hosts no tutorials of
its own (editorials live on oi.edu.pl).

---

## Polska Wikipedia — the one redistributable source

**CC BY-SA**, so it is the only item here that could legally be included in a
public repo with attribution. Useful quick definitions, thin on worked
examples:

[Algorytm Euklidesa](https://pl.wikipedia.org/wiki/Algorytm_Euklidesa) ·
[Przeszukiwanie wszerz](https://pl.wikipedia.org/wiki/Przeszukiwanie_wszerz) ·
[Programowanie dynamiczne](https://pl.wikipedia.org/wiki/Programowanie_dynamiczne)
