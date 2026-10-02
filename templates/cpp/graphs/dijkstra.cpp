// Dijkstra — single-source shortest path, non-negative weights. O(m log n).
//
// WHY THIS FILE EXISTS: KACTL omits Dijkstra on purpose. Its README says it
// excludes "algorithms that are very common/simple (e.g., Dijkstra)" — it
// assumes you have it memorised. Carry it on paper.
//
// TRAPS:
//   * dist must be long long. Weights up to 1e9 with 1e5 edges overflows int.
//   * Use the lazy variant below (push duplicates, skip stale pops). Do not
//     try to decrease-key; it is not worth the code under time pressure.
//   * Unreachable nodes keep dist == INF. Check before printing.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = numeric_limits<ll>::max() / 4;  // /4 leaves room for d + w

// adj[u] = {v, weight}
vector<ll> dijkstra(const vector<vector<pair<int, ll>>> &adj, int src) {
    vector<ll> dist(adj.size(), INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;

    dist[src] = 0;
    pq.emplace(0, src);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;  // stale entry
        for (auto [v, w] : adj[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}

#ifdef TEST_DIJKSTRA
int main() {
    //      (1)      (2)
    //  0 -----> 1 -----> 2
    //  |                 ^
    //  +--------(5)------+       node 3 isolated
    vector<vector<pair<int, ll>>> adj(4);
    auto add = [&](int a, int b, ll w) {
        adj[a].emplace_back(b, w);
        adj[b].emplace_back(a, w);
    };
    add(0, 1, 1);
    add(1, 2, 2);
    add(0, 2, 5);

    auto d = dijkstra(adj, 0);
    assert(d[0] == 0);
    assert(d[1] == 1);
    assert(d[2] == 3);   // 0->1->2 beats the direct edge of 5
    assert(d[3] == INF); // unreachable

    // Overflow guard: a long path of 1e9 weights must not wrap.
    int n = 1000;
    vector<vector<pair<int, ll>>> big(n);
    for (int i = 0; i + 1 < n; i++) {
        big[i].emplace_back(i + 1, 1'000'000'000LL);
        big[i + 1].emplace_back(i, 1'000'000'000LL);
    }
    auto db = dijkstra(big, 0);
    assert(db[n - 1] == 999LL * 1'000'000'000LL);
    puts("Dijkstra ok");
}
#endif
