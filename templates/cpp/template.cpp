// Minimal contest template.
// Build (exactly matches the Kattis judge): make r F=sol
//   g++-15 -g -O2 -std=gnu++23 -o sol sol.cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define all(v) (v).begin(), (v).end()

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<ll> a(n);
    for (ll &x : a) cin >> x;

    cout << accumulate(all(a), 0LL) << '\n';
    return 0;
}
