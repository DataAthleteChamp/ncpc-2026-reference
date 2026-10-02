// Iterative segment tree — range query, point update. O(log n) both.
//
// Use when you need a range operation Fenwick cannot do: min, max, gcd, or
// any associative combine. For plain sums prefer fenwick.cpp (shorter).
//
// The iterative form below is far less code than the recursive one and has
// no lazy propagation -- if you need range UPDATES, that is a different and
// much longer structure; think hard about whether you really need it.
//
// TRAPS:
//   * query is half-open [l, r). Pick one convention and never mix.
//   * identity must be neutral for combine: 0 for sum, +inf for min.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

template <class T>
struct SegTree {
    int n;
    T identity;
    function<T(T, T)> combine;
    vector<T> t;

    SegTree(int n, T identity, function<T(T, T)> combine)
        : n(n), identity(identity), combine(move(combine)), t(2 * n, identity) {}

    SegTree(const vector<T> &a, T identity, function<T(T, T)> combine)
        : n((int)a.size()), identity(identity), combine(move(combine)),
          t(2 * (int)a.size(), identity) {
        for (int i = 0; i < n; i++) t[n + i] = a[i];
        for (int i = n - 1; i > 0; i--) t[i] = this->combine(t[2 * i], t[2 * i + 1]);
    }

    // a[i] = value
    void set(int i, T value) {
        for (t[i += n] = value; i > 1; i >>= 1)
            t[i >> 1] = combine(t[i & ~1], t[i | 1]);
    }

    T get(int i) const { return t[i + n]; }

    // Combine over [l, r) — half-open.
    T query(int l, int r) const {
        T resl = identity, resr = identity;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = combine(resl, t[l++]);
            if (r & 1) resr = combine(t[--r], resr);
        }
        return combine(resl, resr);
    }
};

#ifdef TEST_SEGTREE
int main() {
    vector<ll> a = {5, 1, 3, 9, 2};

    SegTree<ll> sum((int)a.size(), 0LL, [](ll x, ll y) { return x + y; });
    for (int i = 0; i < (int)a.size(); i++) sum.set(i, a[i]);
    assert(sum.query(0, 5) == 20);
    assert(sum.query(1, 4) == 13);  // [1,4) = 1+3+9
    assert(sum.query(2, 3) == 3);
    assert(sum.query(3, 3) == 0);   // empty range

    const ll INF = numeric_limits<ll>::max();
    SegTree<ll> mn(a, INF, [](ll x, ll y) { return min(x, y); });
    assert(mn.query(0, 5) == 1);
    assert(mn.query(2, 5) == 2);
    assert(mn.query(3, 4) == 9);
    assert(mn.query(1, 1) == INF);  // empty -> identity

    mn.set(1, 100);                 // a[1]: 1 -> 100
    assert(mn.query(0, 5) == 2);
    assert(mn.get(1) == 100);

    SegTree<ll> mx(a, numeric_limits<ll>::min(), [](ll x, ll y) { return max(x, y); });
    assert(mx.query(0, 5) == 9);
    assert(mx.query(0, 2) == 5);

    // gcd works too -- any associative operation does.
    vector<ll> g = {12, 18, 24};
    SegTree<ll> gg(g, 0LL, [](ll x, ll y) { return __gcd(x, y); });
    assert(gg.query(0, 3) == 6);
    assert(gg.query(0, 2) == 6);
    assert(gg.query(1, 3) == 6);

    // Overflow guard.
    vector<ll> big(1000, 1'000'000'000LL);
    SegTree<ll> bs(big, 0LL, [](ll x, ll y) { return x + y; });
    assert(bs.query(0, 1000) == 1'000'000'000'000LL);
    puts("segment tree ok");
}
#endif
