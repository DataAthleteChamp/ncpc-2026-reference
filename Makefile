# NCPC contest Makefile — never retype compiler flags under pressure.
#
# The judge uses:  g++-15 15.2.0  -g -O2 -std=gnu++23 -static
# We match it exactly, minus -static (not supported on macOS).
#
#   make r F=a           compile a.cpp, then run it on stdin
#   make t F=a           compile a.cpp, run against every a.*.in and diff
#   make d F=a           debug build: sanitizers + libstdc++ debug iterators
#   make p F=a           run a.cpp under its own #ifdef TEST_* self-test
#   make clean

CXX      := /opt/homebrew/bin/g++-15
JUDGE    := -g -O2 -std=gnu++23
# -Wshadow catches the classic "local n shadows global n" contest bug.
# -Wconversion is deliberately NOT default: it fires constantly on ordinary
# int/size_t mixing and trains you to ignore warnings. Use `make strict` when
# you actually suspect a narrowing bug.
WARN     := -Wall -Wextra -Wshadow
STRICT   := $(WARN) -Wconversion -Wsign-conversion
# Apple's clang is used for sanitizers: Homebrew gcc cannot link libasan here.
SANCXX   := clang++
SAN      := -std=c++20 -g -O1 -fsanitize=address,undefined -D_GLIBCXX_DEBUG

F ?= sol

.PHONY: r t d p strict print refs clean check-toolchain

r: $(F)
	@./$(F)

$(F): $(F).cpp
	@$(CXX) $(JUDGE) $(WARN) -o $@ $< && echo "built $@ (judge flags)"

d: $(F).cpp
	@$(SANCXX) $(SAN) $(WARN) -o $(F).dbg $< && echo "built $(F).dbg (sanitized)"
	@./$(F).dbg

# Run against sample files: a.1.in / a.1.ans, a.2.in / a.2.ans, ...
t: $(F)
	@pass=0; fail=0; \
	for in in $(F).*.in; do \
	  [ -e "$$in" ] || { echo "no $(F).*.in files found"; exit 1; }; \
	  ans=$${in%.in}.ans; \
	  got=$$(./$(F) < $$in); \
	  if [ -f "$$ans" ] && [ "$$got" = "$$(cat $$ans)" ]; then \
	    echo "  PASS $$in"; pass=$$((pass+1)); \
	  else \
	    echo "  FAIL $$in"; \
	    echo "    expected: $$(cat $$ans 2>/dev/null)"; \
	    echo "    got:      $$got"; \
	    fail=$$((fail+1)); \
	  fi; \
	done; \
	echo "$$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

# Self-test a template file, e.g. make p F=templates/cpp/ds/dsu
p:
	@name=$$(basename $(F) | tr 'a-z' 'A-Z'); \
	guard=$$(grep -o '#ifdef TEST_[A-Z_]*' $(F).cpp | head -1 | sed 's/#ifdef //'); \
	$(CXX) $(JUDGE) $(WARN) -D$$guard -o /tmp/selftest $(F).cpp && /tmp/selftest

strict: $(F).cpp
	@$(CXX) $(JUDGE) $(STRICT) -fsyntax-only $< && echo "no narrowing warnings"

# Render this repo's checklists + templates into build/team-reference.pdf
print:
	@python3 scripts/build-print-pdf.py

# Download third-party reference PDFs into the gitignored reference/ dir
refs:
	@./scripts/fetch-references.sh

check-toolchain:
	@echo "judge expects: g++-15 15.2.0 / pypy3 7.3.23 (Python 3.11.15)"
	@printf "local g++-15:  "; $(CXX) --version | head -1
	@printf "local clang++: "; $(SANCXX) --version | head -1
	@printf "local python:  "; python3 --version

clean:
	@rm -f $(F) $(F).dbg *.o a.out; rm -rf *.dSYM build; echo "cleaned"
