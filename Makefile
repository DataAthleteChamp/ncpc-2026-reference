# Contest workflow — NCPC 2026
#
# Solve problems in the repo root, one file per problem: a.cpp, b.cpp, ...
#
#   make new P=a       create a.cpp from the template
#   make run P=a       build + run. Uses a.in if present, else reads stdin
#   make runf P=a      build + run in freopen mode: in.txt -> out.txt
#   make test P=a      run against a.1.in/a.1.ans, a.2.in/a.2.ans, ... and diff
#   make debug P=a     sanitized build (AddressSanitizer + bounds-checked STL)
#   make stress P=a    random tests: a.cpp vs a_brut.cpp until they disagree
#   make submit P=a    pre-submit checks, then show the file to upload
#   make clean
#
# Local builds pass -DLOCAL, which enables dbg(). The judge build never
# defines it, so dbg() vanishes on submission and costs nothing to leave in.
#
# File I/O (in.txt -> out.txt) additionally needs -DUSE_FILES, which only
# `make runf` passes. It is deliberately opt-in: auto-detecting in.txt would
# silently override `./a < a.in` and give you the wrong input mid-contest.
#
# Kattis does NOT define ONLINE_JUDGE, so never guard with
# #ifndef ONLINE_JUDGE -- on Kattis that leaves freopen active.

CXX      := /opt/homebrew/bin/g++-15
JUDGE    := -g -O2 -std=gnu++23
WARN     := -Wall -Wextra -Wshadow
STRICT   := $(WARN) -Wconversion -Wsign-conversion
LOCALDEF := -DLOCAL

# Debug build: bounds-checked STL.
#
# Note: Homebrew GCC on macOS ships neither libasan nor libubsan, so
# -fsanitize=address/undefined cannot link. Apple clang has the sanitizers
# but has no <bits/stdc++.h> and no __gnu_pbds, so it cannot compile the
# template at all. _GLIBCXX_DEBUG is the option that actually works here,
# and it catches the dominant bug class in contests -- container indexing --
# naming the container, the bad index and its real size.
SANFLAGS := -g -O0 -std=gnu++23 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -DLOCAL

P ?= a
N ?= 200
TEMPLATE := templates/cpp/solution.cpp

.PHONY: help new run test debug stress brut submit strict selftest print refs clean check-toolchain contest-ready contest-check prep

help:
	@echo "make new P=a      create a.cpp from the template"
	@echo "make run P=a      build and run (a.in -> a.out if a.in exists)"
	@echo "make test P=a     diff against a.1.in/a.1.ans, a.2.in/a.2.ans, ..."
	@echo "make debug P=a    sanitized build, finds out-of-bounds"
	@echo "make brut P=a     scaffold a_brut.cpp + gen.py for stress testing"
	@echo "make stress P=a   random tests vs a_brut.cpp (N=200 by default)"
	@echo "make submit P=a   pre-submit checks"
	@echo "make selftest     run every template's built-in tests"
	@echo "make print        build cockpit.pdf + team-reference.pdf"
	@echo "make prep         fetch refs AND build both PDFs"
	@echo ""
	@echo "make contest-check  dry run: what must leave the laptop"
	@echo "make contest-ready  remove all prewritten code before the contest"

new:
	@if [ -e $(P).cpp ]; then \
	  echo "$(P).cpp already exists - refusing to overwrite"; exit 1; fi
	@cp $(TEMPLATE) $(P).cpp && echo "created $(P).cpp"

# Local build: dbg() active, in.txt honoured.
$(P): $(P).cpp
	@$(CXX) $(JUDGE) $(WARN) $(LOCALDEF) -o $@ $< && echo "built $@"

run: $(P)
	@if [ -f $(P).in ]; then \
	  echo "--- $(P).in -> $(P).out ---"; \
	  ./$(P) < $(P).in > $(P).out && cat $(P).out; \
	elif [ -f in.txt ]; then \
	  echo "--- in.txt -> out.txt ---"; \
	  ./$(P) < in.txt > out.txt && cat out.txt; \
	else \
	  echo "--- reading stdin (no $(P).in or in.txt) ---"; ./$(P); \
	fi

test: $(P)
	@pass=0; fail=0; \
	found=$$(ls $(P).*.in 2>/dev/null | wc -l | tr -d ' '); \
	if [ "$$found" = "0" ]; then \
	  echo "no $(P).*.in files. Create $(P).1.in and $(P).1.ans"; exit 1; fi; \
	for in in $(P).*.in; do \
	  ans=$${in%.in}.ans; \
	  if [ ! -f "$$ans" ]; then echo "  SKIP $$in (no $$ans)"; continue; fi; \
	  got=$$(./$(P) < $$in 2>/dev/null); \
	  if [ "$$got" = "$$(cat $$ans)" ]; then \
	    echo "  PASS $$in"; pass=$$((pass+1)); \
	  else \
	    echo "  FAIL $$in"; \
	    echo "    expected: $$(cat $$ans)"; \
	    echo "    got:      $$got"; \
	    fail=$$((fail+1)); \
	  fi; \
	done; \
	echo "$$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

debug: $(P).cpp
	@$(CXX) $(SANFLAGS) $(WARN) -o $(P).dbg $< && echo "built $(P).dbg (bounds-checked STL)"
	@if [ -f $(P).in ]; then ./$(P).dbg < $(P).in; else ./$(P).dbg; fi

brut:
	@if [ -e $(P)_brut.cpp ]; then echo "$(P)_brut.cpp already exists"; else \
	  cp $(TEMPLATE) $(P)_brut.cpp; \
	  echo "created $(P)_brut.cpp - write the SLOW, obviously correct version here"; fi
	@if [ -e gen.py ]; then echo "gen.py already exists"; else \
	  cp scripts/gen.py gen.py; \
	  echo "created gen.py - edit it to match the input format"; fi

# Stress testing: the standard olympiad loop.
#   gen.py SEED  ->  random test
#   a_brut.cpp   ->  slow solution you trust (the oracle)
#   a.cpp        ->  the fast solution under test
# Stops at the first disagreement, leaving the counterexample in stress.in.
stress: $(P)
	@test -f $(P)_brut.cpp || { \
	  echo "need $(P)_brut.cpp - run: make brut P=$(P)"; exit 1; }
	@test -f gen.py || { echo "need gen.py - run: make brut P=$(P)"; exit 1; }
	@$(CXX) $(JUDGE) $(WARN) $(LOCALDEF) -o $(P)_brut $(P)_brut.cpp
	@echo "running $(N) random tests..."
	@i=1; \
	while [ $$i -le $(N) ]; do \
	  python3 gen.py $$i > stress.in; \
	  ./$(P) < stress.in > stress.got 2>/dev/null; \
	  ./$(P)_brut < stress.in > stress.exp 2>/dev/null; \
	  if ! diff -qb stress.got stress.exp >/dev/null 2>&1; then \
	    echo ""; \
	    echo "MISMATCH on seed $$i  (reproduce: python3 gen.py $$i)"; \
	    echo "--- input (stress.in) ---"; cat stress.in; \
	    echo "--- yours ---";             cat stress.got; \
	    echo "--- brute force ---";       cat stress.exp; \
	    exit 1; \
	  fi; \
	  i=$$((i+1)); \
	done; \
	echo "all $(N) tests agree"

# Build exactly as the judge does, and check for the classic mistakes.
submit: $(P).cpp
	@echo "building with judge flags (no -DLOCAL)..."
	@$(CXX) $(JUDGE) $(WARN) -o $(P).judge $(P).cpp
	@echo "  compiles clean with judge flags"
	@if grep -n 'freopen' $(P).cpp | grep -qv '//'; then \
	  if ! grep -q '#ifdef LOCAL' $(P).cpp; then \
	    echo "  WARNING: freopen present but no #ifdef LOCAL guard found"; fi; fi
	@if grep -qE 'cout *<< *"(debug|DEBUG)' $(P).cpp; then \
	  echo "  WARNING: debug text appears to go to cout"; fi
	@if [ -f $(P).1.in ]; then $(MAKE) --no-print-directory test P=$(P) || exit 1; fi
	@echo ""
	@echo "submit this file: $(P).cpp"

strict: $(P).cpp
	@$(CXX) $(JUDGE) $(STRICT) -fsyntax-only $< && echo "no narrowing warnings"

selftest:
	@for f in $$(find templates/cpp -name '*.cpp' | sort); do \
	  guard=$$(grep -o '#ifdef TEST_[A-Z0-9_]*' $$f | head -1 | sed 's/#ifdef //'); \
	  [ -n "$$guard" ] || continue; \
	  printf '  %-30s ' "$$(basename $$f .cpp)"; \
	  $(CXX) $(JUDGE) $(WARN) -D$$guard -o /tmp/ncpc_selftest $$f 2>/tmp/ncpc_err \
	    && /tmp/ncpc_selftest || { echo "FAILED"; head -5 /tmp/ncpc_err; }; \
	done

print:
	@python3 scripts/build-cockpit.py
	@python3 scripts/build-print-pdf.py
	@# Polish formula sheet, only if make refs has fetched the source.
	@[ -f reference/pl/wzorki.md ] && python3 scripts/build-pl-formulas.py || true
	@echo ""
	@echo "What to print, and how many copies: print/PRINT-ME.md"

# One command for tonight: fetch references, build both PDFs, list the plan.
prep: refs print
	@echo ""
	@cat print/PRINT-ME.md | sed -n '1,12p'

refs:
	@./scripts/fetch-references.sh

check-toolchain:
	@echo "judge expects: g++-15 15.2.0 / pypy3 7.3.23 (Python 3.11.15)"
	@printf "local g++-15:  "; $(CXX) --version | head -1
	@printf "local python:  "; python3 --version
	@printf "bits/stdc++.h: "; \
	  echo '#include <bits/stdc++.h>' > /tmp/ncpc_hdr.cpp; \
	  echo 'int main(){}' >> /tmp/ncpc_hdr.cpp; \
	  $(CXX) -std=gnu++23 -fsyntax-only /tmp/ncpc_hdr.cpp 2>/dev/null \
	    && echo "ok" || echo "MISSING - wrong compiler?"

clean:
	@rm -f $(P) $(P).bin $(P).dbg $(P).judge $(P)_brut $(P).out
	@rm -f out.txt err.txt stress.in stress.got stress.exp
	@rm -rf *.dSYM build __pycache__ scripts/__pycache__
	@echo "cleaned (sol.cpp and in.txt kept)"

# ---------------------------------------------------------------------------
# Contest compliance
# ---------------------------------------------------------------------------
# NCPC 2026 rules (2026-09-28):
#   BANNED  "Any local digital databases of pre-written code ... old
#            solutions, PDFs of textbooks, copies of webpages"
#   ALLOWED "Physical material has no restrictions: team reference documents,
#            printed code, textbooks ... There is no limit"
#
# So: the paper is legal and unlimited; the files on the laptop are not.
# Run this BEFORE you leave for the venue.
contest-ready:
	@echo "This deletes every prewritten-code file on this machine,"
	@echo "including .git -- the history contains the same templates."
	@echo ""
	@echo "Before you continue:"
	@echo "  1. print/team-reference.pdf must already be PRINTED"
	@echo "  2. everything is recoverable afterwards with:"
	@echo "     git clone https://github.com/DataAthleteChamp/ncpc-2026-reference"
	@echo ""
	@echo "Continue? [y/N]"
	@read ans; [ "$$ans" = "y" ] || { echo "aborted"; exit 1; }
	@rm -rf templates reference books solutions practice scripts
	@rm -rf build print checklists .git .vscode
	@rm -f Makefile README.md START-HERE.md SOURCES.md LICENSE sol.cpp run
	@rm -f $(P) $(P).bin $(P).dbg $(P).judge out.txt err.txt in.txt
	@echo ""
	@echo "Done. No prewritten code remains on this machine."
	@echo "Type your template from the printed copy at the start of the contest."

# Dry run: show exactly what contest-ready would delete.
contest-check:
	@echo "Files that would be REMOVED (prewritten code / references):"
	@for d in templates reference books solutions practice scripts build print checklists; do \
	  [ -e $$d ] && echo "  $$d/"; done; true
	@for f in Makefile README.md START-HERE.md SOURCES.md sol.cpp run; do \
	  [ -e $$f ] && echo "  $$f"; done; true
	@echo ""
	@echo "Still allowed on the machine: compiler, editor, browser."
	@echo "Still allowed on paper: everything, without limit."
