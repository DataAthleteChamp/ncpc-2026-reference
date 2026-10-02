// Elementary number theory — the kind NCPC actually uses.
//
// Across NCPC 2021-2025, number theory appeared in 9 of 57 problems, and it
// was almost always ELEMENTARY: gcd/lcm, modular arithmetic, parity, divisors,
// primes. Nothing exotic. This file covers that range.
//
// TRAPS:
//   * lcm(a,b) overflows easily: divide BEFORE multiplying.
//   * C++'s % returns a NEGATIVE result for negative input. Use mod() below.
//   * modpow needs __int128 (or careful casting) if the modulus exceeds ~3e9.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// C++17 has std::gcd / std::lcm in <numeric>. Hand versions for paper use:
ll gcd_(ll a, ll b) { return b ? gcd_(b, a % b) : a; }

// Divide first — a*b would overflow for a,b near 1e9.
ll lcm_(ll a, ll b) { return a / gcd_(a, b) * b; }

// Always-nonnegative modulo. C++'s % gives -1 for (-1 % 5).
ll mod(ll a, ll m) { return ((a % m) + m) % m; }

// (base^exp) % m in O(log exp).
ll modpow(ll base, ll exp, ll m) {
    base = mod(base, m);
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result = (ll)((__int128)result * base % m);
        base = (ll)((__int128)base * base % m);
        exp >>= 1;
    }
    return result;
}

// Modular inverse, for PRIME m only (Fermat's little theorem).
ll modinv(ll a, ll m) { return modpow(a, m - 2, m); }

// Extended Euclid: returns g = gcd(a,b) and sets x, y with a*x + b*y = g.
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// General modular inverse — works for ANY m coprime to a, not just prime m.
// Returns -1 when no inverse exists (gcd(a,m) != 1).
ll modinvGeneral(ll a, ll m) {
    ll x, y;
    if (extgcd(mod(a, m), m, x, y) != 1) return -1;
    return mod(x, m);
}

// Smallest prime factor sieve up to n. Also gives primality and factorisation.
vector<int> sieve(int n) {
    vector<int> spf(n + 1);
    iota(spf.begin(), spf.end(), 0);
    for (int i = 2; (ll)i * i <= n; i++)
        if (spf[i] == i)
            for (int j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
    return spf;
}

// Prime factorisation by trial division, O(sqrt(n)). Fine up to ~1e12.
vector<pair<ll, int>> factor(ll n) {
    vector<pair<ll, int>> f;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p) continue;
        int e = 0;
        while (n % p == 0) n /= p, e++;
        f.emplace_back(p, e);
    }
    if (n > 1) f.emplace_back(n, 1);  // leftover prime
    return f;
}

// All divisors of n, unsorted. O(sqrt(n)).
vector<ll> divisors(ll n) {
    vector<ll> d;
    for (ll i = 1; i * i <= n; i++)
        if (n % i == 0) {
            d.push_back(i);
            if (i != n / i) d.push_back(n / i);
        }
    return d;
}

bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p = 2; p * p <= n; p++)
        if (n % p == 0) return false;
    return true;
}

#ifdef TEST_NUMBER_THEORY
int main() {
    assert(gcd_(12, 18) == 6);
    assert(gcd_(17, 5) == 1);
    assert(lcm_(4, 6) == 12);
    // Overflow guard: naive a*b/gcd would wrap, dividing first does not.
    assert(lcm_(1'000'000'000LL, 999'999'998LL) == 499999999000000000LL);

    // C++ native % would give -1 here.
    assert(mod(-1, 5) == 4);
    assert(mod(-13, 5) == 2);
    assert(mod(13, 5) == 3);

    assert(modpow(2, 10, 1'000'000'007) == 1024);
    assert(modpow(2, 0, 7) == 1);
    // Large modulus: plain 64-bit multiply would overflow, __int128 does not.
    assert(modpow(123456789, 123456789, 1'000'000'007LL) == 907408795LL);
    assert(modinv(3, 7) == 5 && 3 * 5 % 7 == 1);

    // Extended Euclid: the Bezout identity must actually hold.
    ll x, y;
    assert(extgcd(240, 46, x, y) == 2);
    assert(240 * x + 46 * y == 2);
    // General inverse works where Fermat's does not: 10 is composite.
    assert(modinvGeneral(3, 10) == 7 && 3 * 7 % 10 == 1);
    assert(modinvGeneral(3, 7) == 5);            // agrees with Fermat on primes
    assert(modinvGeneral(4, 10) == -1);          // gcd(4,10)=2, no inverse exists

    auto spf = sieve(50);
    assert(spf[2] == 2 && spf[3] == 3 && spf[4] == 2 && spf[49] == 7);
    int primes = 0;
    for (int i = 2; i <= 50; i++)
        if (spf[i] == i) primes++;
    assert(primes == 15);  // primes below 50

    auto f = factor(360);  // 2^3 * 3^2 * 5
    assert(f.size() == 3);
    assert(f[0] == make_pair(2LL, 3));
    assert(f[1] == make_pair(3LL, 2));
    assert(f[2] == make_pair(5LL, 1));
    assert(factor(999'999'937LL).size() == 1);  // large prime

    auto d = divisors(36);
    sort(d.begin(), d.end());
    assert((d == vector<ll>{1, 2, 3, 4, 6, 9, 12, 18, 36}));

    assert(isPrime(2) && isPrime(999'999'937LL));
    assert(!isPrime(1) && !isPrime(0) && !isPrime(561));
    puts("number theory ok");
}
#endif
