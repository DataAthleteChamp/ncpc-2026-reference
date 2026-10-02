// Base solution template — NCPC 2026.
//
//   make new P=a      create work/a.cpp from this file
//   make run P=a      build + run with work/a.in  -> work/a.out
//   make test P=a     run every work/a.*.in against work/a.*.ans
//   make debug P=a    sanitizers + bounds-checked STL
//   make stress P=a   random tests vs a brute force
//
// -------------------------------------------------------------------------
// WHY dbg() WRITES TO cerr, NOT cout
//
// A debug macro that prints to cout writes into the answer the judge reads.
// It will not look wrong locally, because locally you read your own output.
// On Kattis it is a silent Wrong Answer, and you will burn submissions
// hunting an algorithmic bug that is not there.
//
// cerr is a separate stream; Kattis ignores it. And because dbg is wrapped
// in #ifdef LOCAL, it compiles to literally nothing unless you build with
// -DLOCAL. The Makefile passes -DLOCAL for local runs and never for the
// file you submit, so leaving dbg() calls in your submission is harmless.
// -------------------------------------------------------------------------
#include <bits/stdc++.h>

// Policy-based data structures: ordered_set supports "how many elements are
// less than x" and "what is the k-th element" in O(log n).
// GCC only -- these headers do not exist on Apple clang.
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

template <class T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define pb push_back
#define eb emplace_back
#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()
#define sz(v) ((int)(v).size())

// endl is a function that FLUSHES. In a loop that alone can cause TLE.
// Redefining it to a plain newline removes the flush everywhere at once.
#define endl '\n'

// ---------------------------- debug output -------------------------------
#ifdef LOCAL
template <class T, class = decltype(begin(declval<T>())),
          class = enable_if_t<!is_same_v<T, string>>>
ostream &operator<<(ostream &os, const T &c);
template <class A, class B>
ostream &operator<<(ostream &os, const pair<A, B> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}
template <class T, class, class>
ostream &operator<<(ostream &os, const T &c) {
    os << '{';
    for (auto it = begin(c); it != end(c); ++it)
        os << (it == begin(c) ? "" : ", ") << *it;
    return os << '}';
}

inline void dbg_out() { cerr << '\n'; }
template <class Head, class... Tail>
void dbg_out(Head H, Tail... T) {
    cerr << ' ' << H;
    dbg_out(T...);
}
#define dbg(...) \
    cerr << "[" << __LINE__ << "] (" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...) ((void)0)
#endif
// -------------------------------------------------------------------------

void solve() {
    int n;
    cin >> n;

    vll a(n);
    for (ll &x : a) cin >> x;

    dbg(n, a);  // vanishes unless built with -DLOCAL

    cout << accumulate(all(a), 0LL) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#if defined(LOCAL) && defined(USE_FILES)
    // Opt-in file I/O: `make runf P=a` passes -DLOCAL -DUSE_FILES and reads
    // in.txt / writes out.txt. Plain `make run P=a` uses shell redirection.
    //
    // This is deliberately NOT automatic. Auto-detecting in.txt would
    // silently override `./sol < a.in`, which is exactly the kind of
    // surprise you cannot afford mid-contest.
    //
    // Neither LOCAL nor USE_FILES is ever defined by a judge -- note that
    // Kattis does NOT define ONLINE_JUDGE, so the common #ifndef ONLINE_JUDGE
    // guard silently leaves freopen active on Kattis. Use LOCAL.
    if (!freopen("in.txt", "r", stdin)) {
        cerr << "in.txt not found\n";
        return 1;
    }
    if (!freopen("out.txt", "w", stdout)) {
        cerr << "cannot open out.txt for writing\n";
        return 1;
    }
#endif

    // Multi-test input? Replace the body with:
    //   int t; cin >> t; while (t--) solve();
    solve();
    return 0;
}
