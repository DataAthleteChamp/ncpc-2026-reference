// String algorithms. Strings are uncommon at NCPC (4 of 57 problems,
// 2021-2025) and when they appear it is usually hashing or a simple scan --
// not suffix automata. These cover that realistic range.
//
// TRAPS:
//   * Hashing: use a RANDOM base. Fixed bases get anti-hash tested.
//   * Hashing collisions are possible; use a 64-bit modulus.
//   * KMP's failure function is 0-indexed here; off-by-ones are easy.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

// Prefix function (KMP): pi[i] = length of the longest proper prefix of
// s[0..i] that is also a suffix of it. O(n).
vector<int> prefixFunction(const string &s) {
    vector<int> pi(s.size(), 0);
    for (size_t i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// All start positions of `pat` inside `text`, 0-indexed. O(n + m).
vector<int> findAll(const string &text, const string &pat) {
    if (pat.empty() || pat.size() > text.size()) return {};
    string combined = pat + '\x01' + text;
    vector<int> pi = prefixFunction(combined), res;
    for (size_t i = pat.size() + 1; i < combined.size(); i++)
        if (pi[i] == (int)pat.size())
            res.push_back((int)(i - 2 * pat.size()));
    return res;
}

// Z-function: z[i] = length of the longest common prefix of s and s[i..].
vector<int> zFunction(const string &s) {
    int n = (int)s.size();
    vector<int> z(n, 0);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}

// Polynomial rolling hash with O(1) substring queries.
// Random base chosen at construction defeats anti-hash tests.
struct StringHash {
    static const ull MOD = (1ULL << 61) - 1;  // Mersenne prime
    ull base;
    vector<ull> h, p;

    static ull mulmod(ull a, ull b) { return (ull)((__uint128_t)a * b % MOD); }

    explicit StringHash(const string &s) {
        static mt19937_64 rng(
            (ull)chrono::steady_clock::now().time_since_epoch().count());
        base = uniform_int_distribution<ull>(256, MOD - 2)(rng);

        h.assign(s.size() + 1, 0);
        p.assign(s.size() + 1, 1);
        for (size_t i = 0; i < s.size(); i++) {
            h[i + 1] = (mulmod(h[i], base) + (ull)s[i]) % MOD;
            p[i + 1] = mulmod(p[i], base);
        }
    }

    // Hash of s[l..r] inclusive, 0-indexed.
    ull get(int l, int r) const {
        return (h[r + 1] + MOD - mulmod(h[l], p[r - l + 1])) % MOD;
    }
};

bool isPalindrome(const string &s) {
    for (size_t i = 0, j = s.size(); i + 1 < j; i++, j--)
        if (s[i] != s[j - 1]) return false;
    return true;
}

// Split on a single delimiter. Keeps empty fields, like Python's str.split(c).
vector<string> split(const string &s, char delim) {
    vector<string> out;
    string cur;
    istringstream iss(s);
    while (getline(iss, cur, delim)) out.push_back(cur);
    return out;
}

#ifdef TEST_STRINGS
int main() {
    auto pi = prefixFunction("aabaaab");
    assert((pi == vector<int>{0, 1, 0, 1, 2, 2, 3}));
    assert(prefixFunction("").empty());

    assert((findAll("ababab", "ab") == vector<int>{0, 2, 4}));
    assert((findAll("aaaa", "aa") == vector<int>{0, 1, 2}));
    assert(findAll("abc", "xyz").empty());
    assert(findAll("abc", "").empty());
    assert(findAll("a", "abc").empty());

    auto z = zFunction("aabxaab");
    assert(z[0] == 7 && z[1] == 1 && z[4] == 3);
    assert(zFunction("").empty());

    string s = "abracadabra";
    StringHash hs(s);
    assert(hs.get(0, 3) == hs.get(7, 10));  // "abra" twice
    assert(hs.get(0, 0) != hs.get(1, 1));   // 'a' vs 'b'
    assert(hs.get(0, 10) == hs.get(0, 10));

    // Same substring at different offsets must hash equal.
    string t = "xxabcxxabc";
    StringHash ht(t);
    assert(ht.get(2, 4) == ht.get(7, 9));

    assert(isPalindrome("racecar"));
    assert(isPalindrome("abba"));
    assert(isPalindrome(""));
    assert(isPalindrome("a"));
    assert(!isPalindrome("abc"));

    assert((split("a,b,c", ',') == vector<string>{"a", "b", "c"}));
    assert((split("a", ',') == vector<string>{"a"}));
    assert((split("a,,b", ',') == vector<string>{"a", "", "b"}));
    puts("strings ok");
}
#endif
