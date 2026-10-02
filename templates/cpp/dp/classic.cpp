// Classic DP patterns. DP is the most common named technique at NCPC
// (10 of 57 problems, 2021-2025), but it is almost always a variation on
// one of these shapes rather than a textbook recurrence.
//
// HOW TO RECOGNISE DP: you are asked for a count, a minimum/maximum, or
// "is it possible", AND the problem has a natural "first i items" or
// "prefix of the string" structure with overlapping subproblems.
//
// TRAPS:
//   * Use long long for counts -- they explode.
//   * Initialise carefully: 0, -infinity and "impossible" are different.
//   * Loop ORDER decides 0/1 knapsack vs unbounded. See below.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// 0/1 knapsack: each item used at most once. O(n * capacity).
// Iterate capacity DOWNWARD so each item is counted once.
ll knapsack01(const vector<int> &weight, const vector<ll> &value, int capacity) {
    vector<ll> dp(capacity + 1, 0);
    for (size_t i = 0; i < weight.size(); i++)
        for (int c = capacity; c >= weight[i]; c--)
            dp[c] = max(dp[c], dp[c - weight[i]] + value[i]);
    return dp[capacity];
}

// Unbounded knapsack: unlimited copies. Same code, capacity UPWARD.
ll knapsackUnbounded(const vector<int> &weight, const vector<ll> &value, int capacity) {
    vector<ll> dp(capacity + 1, 0);
    for (size_t i = 0; i < weight.size(); i++)
        for (int c = weight[i]; c <= capacity; c++)
            dp[c] = max(dp[c], dp[c - weight[i]] + value[i]);
    return dp[capacity];
}

// Can we hit exactly `target`? Classic subset-sum.
bool subsetSum(const vector<int> &a, int target) {
    vector<char> dp(target + 1, 0);
    dp[0] = 1;
    for (int x : a)
        for (int c = target; c >= x; c--)
            if (dp[c - x]) dp[c] = 1;
    return dp[target] != 0;
}

// Fewest coins summing to target; -1 if impossible.
ll coinChangeMin(const vector<int> &coins, int target) {
    const ll BIG = numeric_limits<ll>::max() / 4;
    vector<ll> dp(target + 1, BIG);
    dp[0] = 0;
    for (int c = 1; c <= target; c++)
        for (int coin : coins)
            if (coin <= c && dp[c - coin] != BIG)
                dp[c] = min(dp[c], dp[c - coin] + 1);
    return dp[target] == BIG ? -1 : dp[target];
}

// Number of ways to make target where order does NOT matter.
// Coins must be the OUTER loop; swapping the loops counts permutations.
ll coinChangeWays(const vector<int> &coins, int target) {
    vector<ll> dp(target + 1, 0);
    dp[0] = 1;
    for (int coin : coins)
        for (int c = coin; c <= target; c++) dp[c] += dp[c - coin];
    return dp[target];
}

// Longest increasing subsequence LENGTH, O(n log n).
// For non-decreasing, use upper_bound instead of lower_bound.
int lis(const vector<ll> &a) {
    vector<ll> tails;
    for (ll x : a) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return (int)tails.size();
}

// Longest common subsequence length. O(n*m).
int lcs(const string &a, const string &b) {
    vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1, 0));
    for (size_t i = 1; i <= a.size(); i++)
        for (size_t j = 1; j <= b.size(); j++)
            dp[i][j] = (a[i - 1] == b[j - 1])
                           ? dp[i - 1][j - 1] + 1
                           : max(dp[i - 1][j], dp[i][j - 1]);
    return dp[a.size()][b.size()];
}

// Edit distance (Levenshtein). O(n*m).
int editDistance(const string &a, const string &b) {
    vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1, 0));
    for (size_t i = 0; i <= a.size(); i++) dp[i][0] = (int)i;
    for (size_t j = 0; j <= b.size(); j++) dp[0][j] = (int)j;
    for (size_t i = 1; i <= a.size(); i++)
        for (size_t j = 1; j <= b.size(); j++)
            dp[i][j] = (a[i - 1] == b[j - 1])
                           ? dp[i - 1][j - 1]
                           : 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
    return dp[a.size()][b.size()];
}

// Maximum subarray sum (Kadane), O(n). Requires at least one element,
// so an all-negative array returns its largest element.
ll maxSubarray(const vector<ll> &a) {
    ll best = numeric_limits<ll>::min(), cur = 0;
    for (ll x : a) {
        cur = max(x, cur + x);
        best = max(best, cur);
    }
    return best;
}

#ifdef TEST_DP
int main() {
    vector<int> w = {2, 3, 4, 5};
    vector<ll> v = {3, 4, 5, 6};
    assert(knapsack01(w, v, 5) == 7);         // 2+3 -> 3+4
    assert(knapsack01(w, v, 0) == 0);
    assert(knapsackUnbounded(w, v, 6) == 9);  // 2+2+2 -> 3+3+3

    assert(subsetSum({3, 34, 4, 12, 5, 2}, 9));
    assert(!subsetSum({3, 34, 4, 12, 5, 2}, 30));
    assert(subsetSum({1}, 0));

    assert(coinChangeMin({1, 5, 10, 25}, 30) == 2);
    assert(coinChangeMin({5, 10}, 3) == -1);
    assert(coinChangeMin({2}, 0) == 0);
    assert(coinChangeWays({1, 2, 5}, 5) == 4);  // 11111, 1112, 122, 5
    assert(coinChangeWays({2}, 3) == 0);

    assert(lis({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    assert(lis({}) == 0);
    assert(lis({7}) == 1);
    assert(lis({5, 4, 3}) == 1);

    assert(lcs("ABCBDAB", "BDCABA") == 4);
    assert(lcs("abc", "abc") == 3);
    assert(lcs("abc", "") == 0);

    assert(editDistance("kitten", "sitting") == 3);
    assert(editDistance("", "abc") == 3);
    assert(editDistance("same", "same") == 0);

    assert(maxSubarray({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    assert(maxSubarray({-5, -2, -8}) == -2);
    // Overflow guard: 1e5 values of 1e9 sums to 1e14.
    assert(maxSubarray(vector<ll>(100000, 1'000'000'000LL)) == 100'000'000'000'000LL);
    puts("dp ok");
}
#endif
