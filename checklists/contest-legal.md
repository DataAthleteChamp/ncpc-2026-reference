# Contest-legal setup

**Read this before Saturday.** The rules changed on 2026-09-28 and this
repository has to be handled differently from an ordinary code library.

## What the rules actually say

Two sentences decide everything. Both are quoted verbatim from
<https://nordic.icpc.io/ncpc2026/> → rules.

**Banned — digital:**

> "Any local digital databases of pre-written code are similarly disallowed.
> This includes old solutions, PDFs of textbooks, copies of webpages and other
> such things."

**Allowed — physical:**

> "Physical material has no restrictions: team reference documents, printed
> code, textbooks, and other such material is allowed. There is no limit on
> the amount of such material."

And the jury's own summary of the change:

> "Probably the most impactful change to teams is that prewritten code must
> now be printed, like at NWERC."

**So prewritten code is not banned. It must be on paper.** "Printed code" and
"team reference documents" are both named as allowed, without limit.

## What that means for this repository

| Item | Saturday |
|---|---|
| `print/team-reference.pdf` **printed on paper** | ✅ Explicitly allowed |
| KACTL and other references **printed** | ✅ Explicitly allowed |
| `templates/` on the laptop | ❌ A local digital database of prewritten code |
| `reference/*.pdf` on the laptop | ❌ Named directly: "PDFs of textbooks" |
| `.git` on the laptop | ❌ The history contains the same templates |
| `sol.cpp` on the laptop | ⚠️ Gray. Treat as banned — see below |
| Compiler, editor, browser | ✅ Allowed |

### On `sol.cpp`

A single boilerplate file is arguably not a "database" of prewritten code. But
the jury rewrote these rules specifically because people were finding things
that were "technically allowed" but against the spirit, and the stated
rationale is that "competitors should write their own code manually."

Do not spend Saturday morning litigating a gray area. **Type the template at
the start of the contest.** It takes about two minutes, it is unambiguously
legal, and it is what teams at NWERC do.

## The procedure

### Tonight

```bash
make refs      # download the third-party references
make print     # regenerate print/team-reference.pdf
```

Print:

1. `print/team-reference.pdf` — your templates and checklists, 24 pages
2. `reference/en/kactl.pdf` — 26 pages
3. The T-Olimpians STL sheet — 8 pages
4. Anything else you want. There is no limit.

### Before leaving for the venue

```bash
make contest-check     # dry run: shows what would be deleted
make contest-ready     # actually delete it
```

This removes `templates/`, `reference/`, `books/`, `.git`, `sol.cpp` and the
rest. Everything is recoverable afterwards:

```bash
git clone https://github.com/DataAthleteChamp/ncpc-2026-reference
```

Also disable, at the settings level, any AI plugin in your editor — Copilot,
Codeium, Cursor tab-complete. These are explicitly disallowed. Turning off the
visible suggestions is not enough; disable the extension.

### At 11:00

Type your template from the printed copy. Practise this once tonight so it is
muscle memory rather than a five-minute fumble.

## Practising the typed template

Time yourself. Target: under two minutes for the skeleton you actually need.

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define all(v) (v).begin(), (v).end()

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // solve

    return 0;
}
```

That is the minimum that matters: the header, the fast-I/O pair, `ll`. Add
`dbg` only if you want it — and if you do, remember it must write to `cerr`,
never `cout`.

The Polish olympiad trainers at
[T-Olimpians](https://t-olimpians.com/wkuwanie) treat typing your template and
test script from memory in two minutes as a trained skill in its own right
("Nauka Szablonów"). Under these rules it is exactly the right skill.

## If you are unsure

Ask the jury. There is a clarification system during the contest, a Discord
before it (<https://discord.gg/8mDKWnBJUY>), and the Copenhagen site directors
are reachable by email. An answer from the jury beats anyone's interpretation,
including this document's.
