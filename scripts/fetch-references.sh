#!/usr/bin/env bash
# Download freely available reference PDFs for printing.
#
# These files are NOT committed to this repository — they belong to their
# authors and have varying licenses. See SOURCES.md. This script fetches them
# to reference/, which is gitignored.
#
# NCPC 2026 RULE: print these, then DELETE them from the contest machine.
# Printed material is unlimited; digital prewritten code and textbook PDFs
# on the laptop are banned as of the 2026-09-28 rule change.
set -euo pipefail

cd "$(dirname "$0")/.."
mkdir -p reference/en reference/pl

get() {
    local url=$1 dest=$2 desc=$3
    printf '  %-34s ' "$desc"
    # Some university servers (notably mimuw.edu.pl) 403 a bare curl, so send
    # a normal browser User-Agent.
    if curl -sSLf --max-time 120 \
        -A "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120 Safari/537.36" \
        -o "$dest" "$url"; then
        printf 'ok (%s)\n' "$(du -h "$dest" | cut -f1)"
    else
        printf 'FAILED — fetch manually: %s\n' "$url"
    fi
}

echo "Fetching English references -> reference/en/"
get "https://raw.githubusercontent.com/kth-competitive-programming/kactl/main/kactl.pdf" \
    "reference/en/kactl.pdf" "KACTL (KTH, 26 pp)"
get "https://cses.fi/book/book.pdf" \
    "reference/en/cses-handbook.pdf" "CSES Handbook (296 pp)"
get "https://raw.githubusercontent.com/jaehyunp/stanfordacm/master/notebook.pdf" \
    "reference/en/stanford-notebook.pdf" "Stanford ACM notebook (20 pp)"
get "https://raw.githubusercontent.com/SuprDewd/CompetitiveProgramming/main/comprog.pdf" \
    "reference/en/suprdewd-comprog.pdf" "Reykjavik U comprog (27 pp)"

echo
echo "Fetching Polish references -> reference/pl/"
get "https://www.mimuw.edu.pl/~idziaszek/algonotes/looking-for-a-challenge-2-pl.pdf" \
    "reference/pl/w-poszukiwaniu-wyzwan-2.pdf" "W poszukiwaniu wyzwan 2 (212 pp)"
get "https://raw.githubusercontent.com/PatrykFlama/UWr/main/Sem6/AP/wzorki.md" \
    "reference/pl/wzorki.md" "UWr wzorki.md (formula sheet)"
get "https://oi.edu.pl/media/attachment/20171006/oi23.pdf" \
    "reference/pl/oi23.pdf" "XXIII Olimpiada Inf. (252 pp)"

echo
echo "  NOTE: T-Olimpians 'Struktury danych w C++' is proprietary and cannot"
echo "  be downloaded here. Use your own copy. See reference/pl/README.md."

cat <<'EOF'

Done. Next:
  1. Print your Tier-1 material — see reference/INDEX.md and reference/pl/README.md
  2. make print            # renders this repo's own templates + checklists
  3. rm -rf reference/     # BEFORE the contest. Paper only.
EOF
