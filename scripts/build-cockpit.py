#!/usr/bin/env python3
"""Build the cockpit sheet: two A4 pages that stay on the desk all contest.

Everything here is consulted repeatedly under time pressure, so it is laid
out for scanning, not reading: dense two-column, no prose, no page turns.

Page 1  what to type, what algorithm fits, what overflows
Page 2  what to check when the judge says no, and how to pace the day

Usage:  python3 scripts/build-cockpit.py   (or: make print)
Output: print/cockpit.pdf
"""
from __future__ import annotations

import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"

DOC = r"""
\documentclass[9pt,a4paper]{extarticle}
\usepackage[margin=1cm]{geometry}
\usepackage{multicol,listings,xcolor,booktabs,array,tcolorbox}
\usepackage{fontspec}
\setmainfont{Helvetica Neue}[Scale=0.92]
\setmonofont{Menlo}[Scale=0.80]
\usepackage{amssymb}

\definecolor{hd}{RGB}{20,40,90}
\definecolor{warn}{RGB}{150,30,30}
\definecolor{ok}{RGB}{20,100,50}
\definecolor{lt}{gray}{0.93}

\setlength{\parindent}{0pt}
\setlength{\columnsep}{14pt}
\setlength{\tabcolsep}{3pt}
\renewcommand{\arraystretch}{1.08}
\pagestyle{empty}

\lstdefinestyle{tiny}{
  basicstyle=\ttfamily\scriptsize, language=C++,
  keywordstyle=\color{hd}\bfseries, commentstyle=\color{ok}\itshape,
  showstringspaces=false, columns=fullflexible, keepspaces=true,
  breaklines=true, aboveskip=2pt, belowskip=2pt,
}
\lstloadlanguages{C++}

% Section heading: coloured rule + title.
\newcommand{\hd}[1]{%
  \vspace{3pt}{\color{hd}\rule{\linewidth}{1.1pt}}\\[1pt]%
  {\bfseries\small\color{hd}\MakeUppercase{#1}}\\[2pt]}

\newcommand{\warnbox}[1]{%
  \begin{tcolorbox}[colback=warn!5,colframe=warn!60,boxrule=0.5pt,
    left=3pt,right=3pt,top=2pt,bottom=2pt,arc=1pt]
  \scriptsize #1\end{tcolorbox}}

\begin{document}

% ======================= PAGE 1 =======================
\begin{center}
{\LARGE\bfseries\color{hd} NCPC 2026 — COCKPIT}\qquad
{\small ITU Copenhagen · Sat 3 Oct · 11:00–16:00 · \textbf{11 problems, not sorted by difficulty}}
\end{center}
\vspace{-4pt}

\begin{multicols}{2}

\hd{1. Type this first (\textasciitilde 2 min)}
\begin{lstlisting}[style=tiny]
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define all(v) (v).begin(),(v).end()

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n; cin >> n;
  vector<ll> a(n);
  for (ll &x : a) cin >> x;

  cout << ans << '\n';
}
\end{lstlisting}
\warnbox{\textbf{Debug must go to \texttt{cerr}, never \texttt{cout}.}
\texttt{cout} debug lands in the answer the judge reads $\Rightarrow$ silent
Wrong Answer that looks like an algorithm bug.\\[2pt]
\texttt{cerr << "dbg " << x << '\textbackslash n';}}

\hd{2. What fits in the time limit}
{\scriptsize Assume $10^8$ ops/sec. Read $n$, find the row, pick the algorithm
\textbf{before} writing code.}

{\scriptsize\begin{tabular}{@{}ll@{}}
\toprule
\textbf{max n} & \textbf{allowed} \\
\midrule
$\le 10$      & $O(n!)$ permutations \\
$\le 20$      & $O(2^n n)$ bitmask / subsets \\
$\le 100$     & $O(n^3)$ Floyd--Warshall \\
$\le 1\,000$  & $O(n^2)$ pairwise, 2D DP \\
$\le 10^5$    & $O(n\log n)$ \textbf{sort/set/bsearch} \\
$\le 10^6$    & $O(n)$ one pass, prefix sums \\
$\le 10^9$    & $O(\log n)$, $O(\sqrt n)$ \\
$\ge 10^{18}$ & $O(1)$ formula only \\
\bottomrule
\end{tabular}}

\vspace{3pt}
{\tiny\textbf{$n\le10^5$ almost always means: sort it, or put it in a
set/map, or binary search it.}}

\hd{3. Read the constraint backwards}
{\tiny
$n\le20$ $\rightarrow$ subset/permutation search. Stop hunting for polynomial.\\
``min $x$ such that'' / ``max $x$ such that'' $\rightarrow$ \textbf{binary search the answer}; you only need a feasibility check.\\
Huge coordinates, few points $\rightarrow$ coordinate compression.\\
Sum of $n$ bounded over cases $\rightarrow$ per-case must be near-linear.\\
$n\le5000$, 2D feel $\rightarrow$ $O(n^2)$ DP.}

\hd{4. Overflow}
{\scriptsize\begin{tabular}{@{}ll@{}}
\toprule
\texttt{int} & $\approx 2.1\times10^9$ \\
\texttt{long long} & $\approx 9.2\times10^{18}$ \\
\texttt{double} & exact ints to $2^{53}\approx9\times10^{15}$ \\
\bottomrule
\end{tabular}}

\vspace{2pt}
\warnbox{\textbf{The \#1 cause of WA at this level.}
$n(n{-}1)/2$ overflows \texttt{int} at $n\approx65\,536$ — far below a
typical $n\le10^5$ bound. Any intermediate past $2\times10^9$:
\texttt{long long}. Write \texttt{1LL << k}, not \texttt{1 << k}.}

\hd{5. Constant factors that cause TLE}
{\tiny
\texttt{cin/cout} without \texttt{sync\_with\_stdio(false)} — up to \textbf{10$\times$}\\
\texttt{endl} instead of \texttt{'\textbackslash n'} — flushes every line\\
\texttt{map}/\texttt{set} — 5--10$\times$ slower than sorting a vector\\
\texttt{unordered\_map} — $O(n)$ worst case under anti-hash tests\\
\texttt{v.erase(v.begin())} in a loop — accidental $O(n^2)$\\
passing containers by value — add \texttt{const\&}}

\hd{6. STL you will actually use}
{\scriptsize\begin{tabular}{@{}ll@{}}
\texttt{sort(all(v))} & $O(n\log n)$ \\
\texttt{sort(all(v),greater<>())} & descending \\
\texttt{reverse(all(v))} & \\
\texttt{v.erase(unique(all(v)),v.end())} & dedupe sorted \\
\texttt{lower\_bound(all(v),x)-v.begin()} & first $\ge x$ \\
\texttt{upper\_bound(all(v),x)-v.begin()} & first $>x$ \\
\texttt{accumulate(all(v),0LL)} & sum \\
\texttt{max\_element(all(v))} & iterator \\
\texttt{count(all(v),x)} & \\
\texttt{\_\_gcd(a,b)} & \\
\texttt{to\_string(x)} / \texttt{stoll(s)} & \\
\texttt{s.substr(i,len)} & \\
\end{tabular}}

\vspace{2pt}
{\tiny\texttt{priority\_queue<T,vector<T>,greater<>>} = min-heap.\\
\texttt{for (auto \&[k,v] : mp)} iterates a map in key order.}

\hd{7. Problem shape $\rightarrow$ technique}
{\scriptsize\begin{tabular}{@{}p{0.44\linewidth}p{0.42\linewidth}@{}}
\toprule
\textbf{The problem says} & \textbf{Try} \\
\midrule
shortest path, all edges cost 1 & BFS \\
shortest path, weights & Dijkstra \\
"are these connected?", grouping & DSU / BFS \\
"min/max $x$ such that \ldots" & binary search answer \\
pick items, maximise value & DP knapsack \\
count the ways & DP, watch overflow \\
"sum of range", many queries & prefix sums \\
range query + updates & Fenwick / segtree \\
best contiguous block & Kadane / two pointers \\
order with dependencies & topological sort \\
pair things up optimally & sort + greedy \\
$n \le 20$, try everything & bitmask \\
points, area, hull & geometry \\
anagram / substring / match & hashing, KMP \\
divisibility, remainders & gcd/lcm, modular \\
game, who wins & think parity / small cases \\
\bottomrule
\end{tabular}}

\vspace{3pt}
{\scriptsize\textbf{If nothing fits:} write a brute force for $n\le8$,
print the answers, and look for the pattern. Several NCPC "easy" problems
are pure observation with no named algorithm at all.}

\hd{8. Snippets worth knowing cold}
\begin{lstlisting}[style=tiny]
// BFS, unweighted shortest path
vector<int> d(n, -1); queue<int> q;
d[s] = 0; q.push(s);
while (!q.empty()) {
  int u = q.front(); q.pop();
  for (int v : adj[u]) if (d[v] < 0) {
    d[v] = d[u] + 1; q.push(v);
  }
}

// binary search the answer
ll lo = 0, hi = 2e18, best = -1;
while (lo <= hi) {
  ll mid = lo + (hi - lo) / 2;   // not (lo+hi)/2
  if (ok(mid)) best = mid, hi = mid - 1;
  else lo = mid + 1;
}

// DSU
int f(int x){ while(p[x]!=x) x=p[x]=p[p[x]]; return x; }
void uni(int a,int b){ a=f(a); b=f(b); if(a!=b) p[a]=b; }

// prefix sums, inclusive [l,r]
vector<ll> P(n+1,0);
for (int i=0;i<n;i++) P[i+1]=P[i]+a[i];
ll s = P[r+1] - P[l];

// grid directions
int dr[]={-1,1,0,0}, dc[]={0,0,-1,1};
\end{lstlisting}

\end{multicols}

\newpage
% ======================= PAGE 2 =======================
\begin{center}
{\LARGE\bfseries\color{warn} THE JUDGE SAID NO}\qquad
{\small Penalty: $+20$ min per wrong submission \emph{on problems you solve}.
Wrong submissions on unsolved problems are \textbf{free}.}
\end{center}
\vspace{-4pt}

\begin{multicols}{2}

\hd{Wrong answer — in this order}
{\tiny
\textbf{1. Re-read the output format.} Singular vs plural, capitalisation,
\texttt{Case \#1:}, trailing text.\\[2pt]
\textbf{2. Overflow?} Any intermediate $>2\times10^9$ $\rightarrow$
\texttt{long long}.\\[2pt]
\textbf{3. $n=0$, $n=1$, empty input?} Single-element cases break more
solutions than anything else.\\[2pt]
\textbf{4. Off-by-one.} 0- vs 1-indexed, \texttt{<} vs \texttt{<=},
inclusive ranges.\\[2pt]
\textbf{5. State reset between test cases?} Clear every global.\\[2pt]
\textbf{6. Floating point.} \texttt{cout << fixed << setprecision(10)}.
Never \texttt{==} on doubles. Prefer integers.\\[2pt]
\textbf{7. Did you misread the problem?} \emph{The most common cause at this
level.} Have your teammate read it independently and say the task back to
you.\\[2pt]
\textbf{8. Ties / multiple valid answers?} Check for a tie-break rule.\\[2pt]
\textbf{9. Do the sample by hand on paper.} If your manual answer differs
from the expected output, you misread the problem.}

\hd{Time limit exceeded}
{\tiny
1. \texttt{sync\_with\_stdio(false); cin.tie(nullptr);} present?\\
2. \texttt{endl} $\rightarrow$ \texttt{'\textbackslash n'}\\
3. Recompute complexity against the \emph{real} constraints\\
4. Accidental $O(n^2)$: erase-in-loop, string \texttt{+=}, by-value args\\
5. \texttt{unordered\_map} $\rightarrow$ \texttt{map} or sorted vector\\
6. Infinite loop / missing visited check}

\hd{Runtime error}
{\tiny
1. \textbf{Out of bounds} — by far the most common\\
2. Stack overflow from recursion ($\approx10^5$ frames) $\rightarrow$ iterative\\
3. Division / modulo by zero\\
4. \texttt{.front()}, \texttt{.top()}, \texttt{.back()} on empty container\\
5. Reading past end of input}

\hd{Before every submission}
{\tiny
$\square$ Compiles with no warnings\\
$\square$ Passes \textbf{every} sample, byte for byte\\
$\square$ Tested $n=1$ and smallest legal input\\
$\square$ Everything past $2\times10^9$ is \texttt{long long}\\
$\square$ No debug output on \texttt{cout}\\
$\square$ \texttt{'\textbackslash n'}, not \texttt{endl}}

\hd{Two people, one keyboard}
{\tiny
\textbf{Coder} writes the current solution.\\[2pt]
\textbf{Reader} does \emph{not} watch the screen — works the next problem on
paper: edge cases, approach, complexity check. Hands over a written plan,
then swap.\\[2pt]
On WA: the \textbf{reader} runs this checklist aloud while the \textbf{coder}
re-reads the statement. Two people, two different inputs.}

\hd{The day}
{\scriptsize\begin{tabular}{@{}ll@{}}
\textbf{10:00} & doors; log in, open scoreboard \\
\textbf{11:00} & \textbf{read all 11. Write nothing.} \\
 & A reads A$\rightarrow$K, B reads K$\rightarrow$A \\
 & mark each E / M / H \\
\textbf{11:20} & start the \textbf{shortest} E, not the nicest \\
\textbf{11:30+} & check scoreboard every 20 min \\
\textbf{12:00} & target 1 solved \\
\textbf{13:00} & target 2--3 \\
\textbf{14:00} & target 3--4 \\
\textbf{15:00} & target 4--5; start nothing new \\
\textbf{15:45} & last realistic submit \\
\end{tabular}}

\vspace{3pt}
\warnbox{\textbf{The problems are NOT sorted by difficulty} — stated verbatim
in every NCPC booklet. In 2025 the two hardest (0.4\% solved) were F and H,
mid-alphabet.\\[3pt]
\textbf{The scoreboard is a free difficulty oracle.} With 230+ teams the easy
problems light up within 15--20 min. If 40 teams solved G and you marked it H,
re-read G — you misjudged it.}

\hd{Reality check}
{\scriptsize Median NCPC team solves \textbf{3--5 of 11}. 15--25\% of teams solve
0 or 1. Every year 1--2 problems are solved by \textbf{zero or one} team out
of 230+. You are not expected to solve the hard half.\\[3pt]
\textbf{Stuck 45 min with no AC? Switch problems.} Come back later.\\[3pt]
Never leave a problem unsubmitted at the end out of fear — a failed
submission at 15:55 on a problem you never solve costs \textbf{nothing}.}

\end{multicols}
\end{document}
"""


def main() -> int:
    if not shutil.which("xelatex"):
        print("error: xelatex not found. Install MacTeX.", file=sys.stderr)
        return 1

    BUILD.mkdir(exist_ok=True)
    tex = BUILD / "cockpit.tex"
    tex.write_text(DOC, encoding="utf-8")

    result = subprocess.run(
        ["xelatex", "-interaction=nonstopmode", "-halt-on-error",
         "-output-directory", str(BUILD), str(tex)],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        print("\nLaTeX failed:\n", file=sys.stderr)
        for line in result.stdout.splitlines():
            if line.startswith("!") or "Error" in line:
                print("  " + line, file=sys.stderr)
        return 1

    out = ROOT / "print" / "cockpit.pdf"
    out.parent.mkdir(exist_ok=True)
    shutil.copyfile(BUILD / "cockpit.pdf", out)
    print(f"-> {out.relative_to(ROOT)} ({out.stat().st_size // 1024} KB)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
