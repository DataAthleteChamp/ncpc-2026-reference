// Binary search on the answer — the highest-value beginner technique.
//
// WHEN TO REACH FOR THIS: the problem says "minimum x such that ..." or
// "maximum x such that ...", and checking a FIXED x is easy. You never have
// to construct the answer, only test candidates. A constraint like
// x <= 1e18 with an easy feasibility check is the tell.
//
// TRAPS:
//   * lo + (hi-lo)/2, not (lo+hi)/2 — the latter overflows.
//   * Get the invariant right: below, check() is monotone false...false,
//     true...true, and we find the FIRST true.
//   * For doubles, iterate a fixed ~100 times instead of comparing.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// First value in [lo, hi] where check is true. Returns hi+1 if never true.
// Requires check to be monotone: once true, true forever after.
ll firstTrue(ll lo, ll hi, const function<bool(ll)> &check) {
    ll result = hi + 1;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;  // overflow-safe
        if (check(mid)) {
            result = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return result;
}

// Last value in [lo, hi] where check is true. Returns lo-1 if never true.
ll lastTrue(ll lo, ll hi, const function<bool(ll)> &check) {
    ll result = lo - 1;
    while (lo <= hi) {
        ll mid = lo + (hi - lo) / 2;
        if (check(mid)) {
            result = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return result;
}

// Real-valued version. 100 iterations is plenty and cannot loop forever.
double binarySearchReal(double lo, double hi, const function<bool(double)> &check) {
    for (int i = 0; i < 100; i++) {
        double mid = (lo + hi) / 2;
        if (check(mid)) hi = mid;
        else lo = mid;
    }
    return lo;
}

#ifdef TEST_BINSEARCH
int main() {
    // Classic: smallest x with x*x >= 1e18.
    ll n = 1'000'000'000'000'000'000LL;
    ll r = firstTrue(0, 2'000'000'000LL, [&](ll x) { return x * x >= n; });
    assert(r == 1'000'000'000LL);

    // Integer square root via lastTrue.
    ll s = lastTrue(0, 2'000'000'000LL, [&](ll x) { return x * x <= 50LL; });
    assert(s == 7);

    // Never true / always true boundaries.
    assert(firstTrue(1, 10, [](ll) { return false; }) == 11);
    assert(firstTrue(1, 10, [](ll) { return true; }) == 1);
    assert(lastTrue(1, 10, [](ll) { return false; }) == 0);
    assert(lastTrue(1, 10, [](ll) { return true; }) == 10);

    // Overflow guard: a (lo+hi)/2 implementation would wrap on this range.
    ll big = firstTrue(0, numeric_limits<ll>::max() / 2,
                       [](ll x) { return x >= 4'611'686'018'427'387'000LL; });
    assert(big == 4'611'686'018'427'387'000LL);

    // Worked example: minimum capacity to ship all weights within D days.
    vector<ll> w = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ll D = 5;
    auto feasible = [&](ll cap) {
        if (cap < *max_element(w.begin(), w.end())) return false;
        ll days = 1, cur = 0;
        for (ll x : w) {
            if (cur + x > cap) days++, cur = 0;
            cur += x;
        }
        return days <= D;
    };
    assert(firstTrue(1, 55, feasible) == 15);

    double d = binarySearchReal(0, 10, [](double x) { return x * x >= 2.0; });
    assert(fabs(d - sqrt(2.0)) < 1e-9);
    puts("binary search ok");
}
#endif
