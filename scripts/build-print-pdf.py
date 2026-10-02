#!/usr/bin/env python3
"""Render this repo's templates and checklists into one print-ready PDF.

NCPC 2026 allows unlimited printed material but bans digital prewritten code,
so the team reference has to exist on paper. This produces that paper.

Usage:  python3 scripts/build-print-pdf.py   (or: make print)
Output: build/team-reference.pdf
"""
from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"

# Order matters: things you grab under pressure go first.
CHECKLISTS = [
    "checklists/complexity-budget.md",
    "checklists/debug.md",
    "checklists/workflow.md",
    "checklists/contest-day.md",
]
TEMPLATES = [
    "templates/cpp/solution.cpp",
    "templates/cpp/binary_search.cpp",
    "templates/cpp/prefix_sums.cpp",
    "templates/cpp/ds/dsu.cpp",
    "templates/cpp/ds/fenwick.cpp",
    "templates/cpp/ds/segment_tree.cpp",
    "templates/cpp/graphs/traversal.cpp",
    "templates/cpp/graphs/dijkstra.cpp",
    "templates/cpp/graphs/algorithms.cpp",
    "templates/cpp/dp/classic.cpp",
    "templates/cpp/math/number_theory.cpp",
    "templates/cpp/strings/strings.cpp",
    "templates/cpp/geometry/geometry.cpp",
    "templates/python/solution.py",
]

PREAMBLE = r"""
\documentclass[9pt,a4paper,twoside]{extarticle}
\usepackage[margin=1.4cm,includefoot]{geometry}
\usepackage{listings,xcolor,fancyhdr,longtable,booktabs,array}
\usepackage[hidelinks]{hyperref}
% XeLaTeX: native UTF-8. Required for both the superscripts used in the
% complexity tables and Polish diacritics (ą ę ł ń ó ś ź ż).
\usepackage{fontspec}
\setmainfont{Helvetica Neue}[Scale=0.95]
\setmonofont{Menlo}[Scale=0.80]
\usepackage{amssymb}

\definecolor{cmt}{rgb}{0.35,0.45,0.35}
\definecolor{kw}{rgb}{0.0,0.0,0.65}
\definecolor{str}{rgb}{0.6,0.1,0.1}
\definecolor{rule}{gray}{0.75}

\lstdefinestyle{code}{
  basicstyle=\ttfamily\scriptsize,
  keywordstyle=\color{kw}\bfseries,
  commentstyle=\color{cmt}\itshape,
  stringstyle=\color{str},
  numbers=left, numberstyle=\tiny\color{gray}, numbersep=5pt,
  breaklines=true, breakatwhitespace=false,
  showstringspaces=false, tabsize=4,
  frame=leftline, framesep=5pt, rulecolor=\color{rule},
  columns=fullflexible, keepspaces=true,
  upquote=true,
}
% Load the language drivers up front; listings cannot load them lazily
% from inside a \begin{lstlisting} that already set style=code.
\lstloadlanguages{C,C++,Python,bash,make}

\pagestyle{fancy}
\fancyhf{}
\fancyhead[L]{\small\textbf{NCPC 2026} -- Team Reference}
\fancyhead[R]{\small\leftmark}
\fancyfoot[C]{\small\thepage}
\renewcommand{\headrulewidth}{0.4pt}

\setcounter{secnumdepth}{2}
\setcounter{tocdepth}{2}

\title{\vspace{-1cm}\textbf{Team Reference Document}\\[2mm]
  \large NCPC 2026 -- Nordic Collegiate Programming Contest}
\date{3 October 2026 \quad|\quad IT-Universitetet i K\o benhavn}
\author{}

\begin{document}
\maketitle
\thispagestyle{fancy}
\vspace{-1.2cm}

\begin{center}
\fbox{\parbox{0.92\textwidth}{\small
\textbf{Judge:} Kattis. C++ compiled with \texttt{g++-15 15.2.0},
\texttt{-g -O2 -std=gnu++23 -static}.
Python 3 is \texttt{pypy3 7.3.23} (Python 3.11.15).\\[1mm]
\textbf{Rules (2026-09-28):} printed material unlimited; \emph{no} digital
prewritten code, no textbook PDFs on the machine, no GenAI/LLM tools.
Live web limited to the contest page, \texttt{cppreference.com} and
\texttt{docs.python.org} (built-in search only).\\[1mm]
\textbf{Scoring:} rank by problems solved; ties by penalty
(minutes to first AC, $+20$ min per wrong submission \emph{on solved
problems}). Wrong submissions on unsolved problems are free.\\[1mm]
\textbf{The problems are not sorted by difficulty.}
}}
\end{center}

\vspace{3mm}
{\small\tableofcontents}
\newpage
"""

LANG = {".cpp": "C++", ".py": "Python", ".h": "C++"}

# Markdown fence labels -> listings language names. Anything unknown falls
# back to no highlighting rather than failing the build.
FENCE_LANG = {
    "cpp": "C++", "c++": "C++", "cc": "C++", "c": "C",
    "py": "Python", "python": "Python",
    "sh": "bash", "bash": "bash", "shell": "bash", "console": "bash",
    "make": "make", "makefile": "make",
}


def tex_escape(s: str) -> str:
    repl = {
        "\\": r"\textbackslash{}", "&": r"\&", "%": r"\%", "$": r"\$",
        "#": r"\#", "_": r"\_", "{": r"\{", "}": r"\}",
        "~": r"\textasciitilde{}", "^": r"\textasciicircum{}",
        # Symbols the text fonts lack a glyph for; render them as maths.
        "\u2192": r"$\rightarrow$", "\u2190": r"$\leftarrow$",
        "\u21d2": r"$\Rightarrow$", "\u2260": r"$\neq$",
        "\u2248": r"$\approx$", "\u00d7": r"$\times$",
        "\u221a": r"$\sqrt{\,}$", "\u2211": r"$\sum$",
        "\u25a1": r"$\square$",
    }
    return "".join(repl.get(c, c) for c in s)


def inline(s: str) -> str:
    """Convert inline markdown to LaTeX, protecting `code` spans."""
    spans: list[str] = []

    def stash(m: re.Match) -> str:
        spans.append(m.group(1))
        return f"\x00{len(spans) - 1}\x00"

    s = re.sub(r"`([^`]+)`", stash, s)
    s = tex_escape(s)
    s = re.sub(r"\*\*([^*]+?)\*\*", r"\\textbf{\1}", s, flags=re.S)
    s = re.sub(r"(?<!\*)\*([^*]+?)\*(?!\*)", r"\\emph{\1}", s, flags=re.S)
    s = re.sub(r"\[([^\]]+)\]\([^)]+\)", r"\1", s)
    for i, code in enumerate(spans):
        s = s.replace(f"\x00{i}\x00", r"\texttt{" + tex_escape(code) + "}")
    return s


def md_table(rows: list[str]) -> str:
    """Render a markdown table as a longtable."""
    def cells(line: str) -> list[str]:
        return [c.strip() for c in line.strip().strip("|").split("|")]

    header = cells(rows[0])
    body = [cells(r) for r in rows[2:] if r.strip()]
    n = len(header)
    spec = "@{}l" + "l" * (n - 2) + "p{0.42\\textwidth}@{}" if n > 2 else "@{}ll@{}"
    if n == 1:
        spec = "@{}l@{}"

    out = [r"\vspace{1mm}", r"{\small\begin{longtable}{" + spec + "}", r"\toprule"]
    out.append(" & ".join(r"\textbf{" + inline(h) + "}" for h in header) + r" \\")
    out.append(r"\midrule\endhead")
    for row in body:
        row = (row + [""] * n)[:n]
        out.append(" & ".join(inline(c) for c in row) + r" \\")
    out += [r"\bottomrule", r"\end{longtable}}"]
    return "\n".join(out)


def md_to_tex(text: str) -> str:
    lines = text.split("\n")
    out: list[str] = []
    i, in_list = 0, False

    def close_list() -> None:
        nonlocal in_list
        if in_list:
            out.append(r"\end{itemize}")
            in_list = False

    while i < len(lines):
        line = lines[i]

        if line.startswith("```"):
            close_list()
            label = line[3:].strip().lower()
            block = []
            i += 1
            while i < len(lines) and not lines[i].startswith("```"):
                block.append(lines[i])
                i += 1
            lang = FENCE_LANG.get(label)
            opts = "style=code,numbers=none"
            if lang:
                opts += ",language=" + lang
            out.append(r"\begin{lstlisting}[" + opts + "]")
            out += block
            out.append(r"\end{lstlisting}")
            i += 1
            continue

        if line.lstrip().startswith("|") and i + 1 < len(lines) and re.match(
            r"^\s*\|[\s:|-]+\|\s*$", lines[i + 1]
        ):
            close_list()
            tbl = []
            while i < len(lines) and lines[i].lstrip().startswith("|"):
                tbl.append(lines[i])
                i += 1
            out.append(md_table(tbl))
            continue

        if m := re.match(r"^(#{1,4})\s+(.*)$", line):
            close_list()
            lvl = len(m.group(1))
            cmd = {1: "section", 2: "subsection", 3: "subsubsection"}.get(lvl, "paragraph")
            out.append("\\" + cmd + "{" + inline(m.group(2)) + "}")
            i += 1
            continue

        if m := re.match(r"^\s*[-*]\s+(?:\[([ x])\]\s*)?(.*)$", line):
            if not in_list:
                out.append(r"\begin{itemize}\itemsep1pt\parskip0pt")
                in_list = True
            box = ""
            if m.group(1) is not None:
                box = r"$\square$\ " if m.group(1) == " " else r"$\boxtimes$\ "
            out.append(r"\item " + box + inline(m.group(2)))
            i += 1
            continue

        if m := re.match(r"^\s*(\d+)\.\s+(.*)$", line):
            if not in_list:
                out.append(r"\begin{itemize}\itemsep1pt\parskip0pt")
                in_list = True
            out.append(r"\item[" + m.group(1) + ".] " + inline(m.group(2)))
            i += 1
            continue

        if line.startswith(">"):
            close_list()
            out.append(r"\begin{quote}\small " + inline(line.lstrip("> ")) + r"\end{quote}")
            i += 1
            continue

        if re.match(r"^\s*---+\s*$", line):
            close_list()
            out.append(r"\vspace{2mm}\hrule\vspace{2mm}")
            i += 1
            continue

        if not line.strip():
            close_list()
            out.append("")
            i += 1
            continue

        # Plain paragraph: gather continuation lines first, so inline markup
        # that wraps across a newline (e.g. **bold text\nmore text**) matches.
        para = [line]
        i += 1
        while i < len(lines):
            nxt = lines[i]
            if (not nxt.strip()
                    or nxt.startswith("```")
                    or nxt.startswith(">")
                    or nxt.lstrip().startswith("|")
                    or re.match(r"^#{1,4}\s+", nxt)
                    or re.match(r"^\s*[-*]\s+", nxt)
                    or re.match(r"^\s*\d+\.\s+", nxt)
                    or re.match(r"^\s*---+\s*$", nxt)):
                break
            para.append(nxt)
            i += 1
        close_list()
        out.append(inline(" ".join(x.strip() for x in para)))
    close_list()
    return "\n".join(out)


def main() -> int:
    if not shutil.which("xelatex"):
        print("error: xelatex not found. Install MacTeX.", file=sys.stderr)
        return 1

    BUILD.mkdir(exist_ok=True)
    doc = [PREAMBLE]

    doc.append(r"\section*{Part I -- Checklists}")
    doc.append(r"\addcontentsline{toc}{section}{Part I -- Checklists}")
    doc.append(r"\markboth{Checklists}{Checklists}")
    for rel in CHECKLISTS:
        p = ROOT / rel
        if not p.exists():
            print(f"  skip (missing): {rel}")
            continue
        doc.append(md_to_tex(p.read_text(encoding="utf-8")))
        doc.append(r"\newpage")
        print(f"  + {rel}")

    doc.append(r"\section*{Part II -- Code templates}")
    doc.append(r"\addcontentsline{toc}{section}{Part II -- Code templates}")
    doc.append(r"\markboth{Templates}{Templates}")
    for rel in TEMPLATES:
        p = ROOT / rel
        if not p.exists():
            print(f"  skip (missing): {rel}")
            continue
        lang = LANG.get(p.suffix, "C++")
        doc.append(r"\subsection{" + tex_escape(rel) + "}")
        doc.append(r"\begin{lstlisting}[style=code,language=" + lang + "]")
        doc.append(p.read_text(encoding="utf-8").rstrip())
        doc.append(r"\end{lstlisting}")
        print(f"  + {rel}")

    doc.append(r"\end{document}")

    tex = BUILD / "team-reference.tex"
    tex.write_text("\n".join(doc), encoding="utf-8")

    # Two passes so the table of contents resolves page numbers.
    for _ in range(2):
        r = subprocess.run(
            ["xelatex", "-interaction=nonstopmode", "-halt-on-error",
             "-output-directory", str(BUILD), str(tex)],
            capture_output=True, text=True,
        )
    if r.returncode != 0:
        print("\nLaTeX failed:\n", file=sys.stderr)
        for ln in r.stdout.splitlines():
            if ln.startswith("!") or "Error" in ln:
                print("  " + ln, file=sys.stderr)
        return 1

    pdf = BUILD / "team-reference.pdf"
    print(f"\n-> {pdf.relative_to(ROOT)} ({pdf.stat().st_size // 1024} KB)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
