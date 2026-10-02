#!/usr/bin/env python3
"""Convert reference/pl/wzorki.md into a printable PDF.

wzorki.md is a condensed formula sheet (extended Euclid, modular inverse,
Euler's totient, CRT, binomials, cross product, polygon area) from the
University of Wroclaw's Algorytmika Praktyczna course.

It is third-party and carries no licence, so the output is written into
reference/pl/ -- which is gitignored. Personal printing only; the PDF is
never committed or redistributed. See SOURCES.md.

Usage:  python3 scripts/build-pl-formulas.py
Output: reference/pl/wzorki.pdf   (gitignored)
"""
from __future__ import annotations

import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "reference" / "pl" / "wzorki.md"
BUILD = ROOT / "build"

HEAD = r"""
\documentclass[9pt,a4paper]{extarticle}
\usepackage[margin=1.2cm]{geometry}
\usepackage{multicol,listings,xcolor,fancyhdr}
\usepackage{fontspec}
\setmainfont{Helvetica Neue}[Scale=0.95]
\setmonofont{Menlo}[Scale=0.80]
\usepackage{amssymb}
\definecolor{hd}{RGB}{20,40,90}
\setlength{\parindent}{0pt}
\pagestyle{fancy}\fancyhf{}
\fancyhead[L]{\small\textbf{Wzory} -- teoria liczb i geometria}
\fancyhead[R]{\small UWr Algorytmika Praktyczna}
\fancyfoot[C]{\small\thepage}
\lstdefinestyle{c}{basicstyle=\ttfamily\scriptsize,showstringspaces=false,
  columns=fullflexible,keepspaces=true,breaklines=true,
  frame=leftline,framesep=4pt,rulecolor=\color{hd!40}}
\begin{document}
\begin{center}{\Large\bfseries\color{hd} Wzory -- teoria liczb i geometria}\\[2pt]
{\small Zrodlo: \texttt{github.com/PatrykFlama/UWr} -- Sem6/AP/wzorki.md\\
Kopia do wlasnego uzytku. Nie rozpowszechniac.}\end{center}
\vspace{4pt}
\begin{multicols}{2}
"""

TAIL = r"""
\end{multicols}
\end{document}
"""

ESC = {"&": r"\&", "%": r"\%", "$": r"\$", "#": r"\#",
       "_": r"\_", "{": r"\{", "}": r"\}",
       "~": r"\textasciitilde{}", "^": r"\textasciicircum{}",
       "\u2192": r"$\rightarrow$", "\u2264": r"$\le$", "\u2265": r"$\ge$",
       "\u2260": r"$\neq$", "\u00d7": r"$\times$", "\u2211": r"$\sum$"}


def esc(s: str) -> str:
    s = s.replace("\\", r"\textbackslash{}")
    return "".join(ESC.get(c, c) for c in s)


def inline(s: str) -> str:
    parts: list[str] = []
    s = re.sub(r"`([^`]+)`", lambda m: (parts.append(m.group(1)), f"\x00{len(parts)-1}\x00")[1], s)
    s = esc(s)
    s = re.sub(r"\*\*([^*]+?)\*\*", r"\\textbf{\1}", s, flags=re.S)
    for i, c in enumerate(parts):
        s = s.replace(f"\x00{i}\x00", r"\texttt{" + esc(c) + "}")
    return s


def convert(md: str) -> str:
    out: list[str] = []
    lines = md.split("\n")
    i = 0
    in_list = False

    def close() -> None:
        nonlocal in_list
        if in_list:
            out.append(r"\end{itemize}")
            in_list = False

    while i < len(lines):
        ln = lines[i]
        if ln.startswith("```"):
            close()
            blk = []
            i += 1
            while i < len(lines) and not lines[i].startswith("```"):
                blk.append(lines[i])
                i += 1
            out.append(r"\begin{lstlisting}[style=c]")
            out += blk
            out.append(r"\end{lstlisting}")
            i += 1
            continue
        if m := re.match(r"^(#{1,4})\s+(.*)$", ln):
            close()
            lvl = len(m.group(1))
            cmd = {1: r"\section*", 2: r"\subsection*", 3: r"\subsubsection*"}.get(lvl, r"\paragraph")
            out.append(cmd + "{" + inline(m.group(2)) + "}")
            i += 1
            continue
        if m := re.match(r"^\s*[-*]\s+(.*)$", ln):
            if not in_list:
                out.append(r"\begin{itemize}\itemsep1pt\parskip0pt")
                in_list = True
            out.append(r"\item " + inline(m.group(1)))
            i += 1
            continue
        if not ln.strip():
            close()
            out.append("")
        else:
            out.append(inline(ln))
        i += 1
    close()
    return "\n".join(out)


def main() -> int:
    if not SRC.exists():
        print(f"error: {SRC.relative_to(ROOT)} missing. Run: make refs", file=sys.stderr)
        return 1
    if not shutil.which("xelatex"):
        print("error: xelatex not found.", file=sys.stderr)
        return 1

    BUILD.mkdir(exist_ok=True)
    tex = BUILD / "wzorki.tex"
    tex.write_text(HEAD + convert(SRC.read_text(encoding="utf-8")) + TAIL, encoding="utf-8")

    r = subprocess.run(
        ["xelatex", "-interaction=nonstopmode", "-halt-on-error",
         "-output-directory", str(BUILD), str(tex)],
        capture_output=True, text=True)
    if r.returncode != 0:
        for line in r.stdout.splitlines():
            if line.startswith("!") or "Error" in line:
                print("  " + line, file=sys.stderr)
        return 1

    out = ROOT / "reference" / "pl" / "wzorki.pdf"
    shutil.copyfile(BUILD / "wzorki.pdf", out)
    print(f"-> {out.relative_to(ROOT)} (gitignored, personal copy)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
