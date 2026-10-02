// Prefix sums — answer "sum of range [l, r]" in O(1) after O(n) setup.
//
// Deceptively high value: many NCPC easy/medium problems reduce to a range
// sum, a running maximum, or a difference-array update. NCPC 2025's "km/h"
// and "Arithmetic Adaptation" were both running-aggregate problems.
//
// TRAPS:
//   * Use long long. Sums of 1e5 values at 1e9 each reach 1e14.
//   * Decide ONCE whether your range is inclusive [l,r] or half-open [l,r).
//     Mixing them is the top source of off-by-one bugs here.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// 1-indexed prefix sums: p[i] = a[0] + ... + a[i-1], so p[0] == 0.
struct Prefix {
    vector<ll> p;

    explicit Prefix(const vector<ll> &a) : p(a.size() + 1, 0) {
        for (size_t i = 0; i < a.size(); i++) p[i + 1] = p[i] + a[i];
    }

    // Inclusive sum over 0-indexed [l, r]. Empty range if l > r.
    ll sum(int l, int r) const { return l > r ? 0 : p[r + 1] - p[l]; }
};

// 2D prefix sums over a grid.
struct Prefix2D {
    vector<vector<ll>> p;

    explicit Prefix2D(const vector<vector<ll>> &g)
        : p(g.size() + 1, vector<ll>(g[0].size() + 1, 0)) {
        for (size_t i = 0; i < g.size(); i++)
            for (size_t j = 0; j < g[0].size(); j++)
                p[i + 1][j + 1] = g[i][j] + p[i][j + 1] + p[i + 1][j] - p[i][j];
    }

    // Inclusive sum over the rectangle (r1,c1)..(r2,c2), 0-indexed.
    ll sum(int r1, int c1, int r2, int c2) const {
        return p[r2 + 1][c2 + 1] - p[r1][c2 + 1] - p[r2 + 1][c1] + p[r1][c1];
    }
};

// Difference array: apply many range-add updates in O(1) each, then build once.
struct Difference {
    vector<ll> d;

    explicit Difference(size_t n) : d(n + 1, 0) {}

    // Add v to every index in [l, r].
    void add(int l, int r, ll v) {
        d[l] += v;
        d[r + 1] -= v;
    }

    vector<ll> build() {
        vector<ll> a(d.size() - 1);
        ll run = 0;
        for (size_t i = 0; i < a.size(); i++) a[i] = (run += d[i]);
        return a;
    }
};

#ifdef TEST_PREFIX
int main() {
    vector<ll> a = {1, 2, 3, 4, 5};
    Prefix pre(a);
    assert(pre.sum(0, 4) == 15);
    assert(pre.sum(1, 3) == 9);
    assert(pre.sum(2, 2) == 3);
    assert(pre.sum(3, 2) == 0);  // empty range

    // Overflow guard: 1e5 elements of 1e9 sums to 1e14, far past int.
    vector<ll> big(100000, 1'000'000'000LL);
    assert(Prefix(big).sum(0, 99999) == 100'000'000'000'000LL);

    vector<vector<ll>> g = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Prefix2D p2(g);
    assert(p2.sum(0, 0, 2, 2) == 45);  // whole grid
    assert(p2.sum(1, 1, 2, 2) == 28);  // 5+6+8+9
    assert(p2.sum(0, 0, 0, 0) == 1);   // single cell
    assert(p2.sum(0, 1, 1, 2) == 16);  // 2+3+5+6

    Difference df(5);
    df.add(0, 2, 10);  // [10,10,10, 0, 0]
    df.add(1, 4, 1);   // [10,11,11, 1, 1]
    df.add(4, 4, 5);   // [10,11,11, 1, 6]
    assert((df.build() == vector<ll>{10, 11, 11, 1, 6}));
    puts("prefix ok");
}
#endif
