// Fenwick tree (binary indexed tree) — prefix sums with point updates.
// Both operations O(log n). Shorter and faster than a segment tree; reach
// for this first when you only need sums.
//
// WHEN: "update one element, query a range sum", repeatedly. If the array
// never changes, use prefix_sums.cpp instead (O(1) queries).
//
// TRAPS:
//   * 1-indexed internally. Public methods below take 0-indexed positions.
//   * Use long long for sums.
//   * add() ADDS a delta. To SET a value you must add (new - old).
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Fenwick {
    int n;
    vector<ll> t;

    explicit Fenwick(int n) : n(n), t(n + 1, 0) {}

    // a[i] += delta, 0-indexed.
    void add(int i, ll delta) {
        for (++i; i <= n; i += i & -i) t[i] += delta;
    }

    // Sum of a[0..i], 0-indexed inclusive. Returns 0 for i < 0.
    ll prefix(int i) const {
        ll s = 0;
        for (++i; i > 0; i -= i & -i) s += t[i];
        return s;
    }

    // Sum of a[l..r], 0-indexed inclusive.
    ll range(int l, int r) const {
        return l > r ? 0 : prefix(r) - prefix(l - 1);
    }

    // Smallest index whose prefix sum is >= target, for non-negative values.
    // Returns n if no such index. O(log n).
    int lowerBound(ll target) const {
        if (target <= 0) return 0;
        int pos = 0;
        ll rem = target;
        for (int pw = 1 << (31 - __builtin_clz(n)); pw > 0; pw >>= 1)
            if (pos + pw <= n && t[pos + pw] < rem) {
                pos += pw;
                rem -= t[pos];
            }
        return pos;
    }
};

#ifdef TEST_FENWICK
int main() {
    vector<ll> a = {5, 1, 3, 9, 2};
    Fenwick f((int)a.size());
    for (int i = 0; i < (int)a.size(); i++) f.add(i, a[i]);

    assert(f.prefix(0) == 5);
    assert(f.prefix(4) == 20);
    assert(f.prefix(-1) == 0);
    assert(f.range(1, 3) == 13);
    assert(f.range(2, 2) == 3);
    assert(f.range(3, 2) == 0);  // empty

    f.add(2, 7);                 // a[2]: 3 -> 10
    assert(f.range(1, 3) == 20);
    assert(f.prefix(4) == 27);

    // Setting a value means adding the difference.
    f.add(0, -5 + 100);          // a[0]: 5 -> 100
    assert(f.range(0, 0) == 100);

    // lowerBound over prefix sums 1,3,6,10,15
    Fenwick g(5);
    for (int i = 0; i < 5; i++) g.add(i, i + 1);
    assert(g.lowerBound(1) == 0);
    assert(g.lowerBound(2) == 1);
    assert(g.lowerBound(6) == 2);
    assert(g.lowerBound(15) == 4);
    assert(g.lowerBound(16) == 5);  // beyond the end

    // Overflow guard: 1e5 values of 1e9 sums past int.
    Fenwick big(100000);
    for (int i = 0; i < 100000; i++) big.add(i, 1'000'000'000LL);
    assert(big.prefix(99999) == 100'000'000'000'000LL);
    puts("fenwick ok");
}
#endif
