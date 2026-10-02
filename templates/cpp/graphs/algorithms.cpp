// More graph algorithms: topological sort, cycle detection, bipartite check,
// Floyd-Warshall, and Kruskal MST.
//
// NCPC uses graphs more than any other category (14 of 57 problems,
// 2021-2025), but almost always as "BFS/Dijkstra plus a twist" rather than
// an exotic algorithm. These are the twists that actually show up.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// Topological order of a DAG (Kahn). Returns {} if the graph has a cycle,
// which doubles as cycle detection for directed graphs.
vector<int> toposort(const vector<vector<int>> &adj) {
    int n = (int)adj.size();
    vector<int> indeg(n, 0), order;
    for (int u = 0; u < n; u++)
        for (int v : adj[u]) indeg[v]++;

    priority_queue<int, vector<int>, greater<>> q;  // smallest-first = deterministic
    for (int u = 0; u < n; u++)
        if (!indeg[u]) q.push(u);

    while (!q.empty()) {
        int u = q.top();
        q.pop();
        order.push_back(u);
        for (int v : adj[u])
            if (--indeg[v] == 0) q.push(v);
    }
    return (int)order.size() == n ? order : vector<int>{};
}

// Undirected cycle detection via DFS, iterative (no stack overflow).
bool hasCycleUndirected(const vector<vector<int>> &adj) {
    int n = (int)adj.size();
    vector<int> parent(n, -2);
    for (int s = 0; s < n; s++) {
        if (parent[s] != -2) continue;
        vector<int> st{s};
        parent[s] = -1;
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            for (int v : adj[u]) {
                if (v == parent[u]) continue;
                if (parent[v] != -2) return true;  // already seen -> cycle
                parent[v] = u;
                st.push_back(v);
            }
        }
    }
    return false;
}

// 2-colour the graph. Returns {} if it is not bipartite (has an odd cycle).
vector<int> bipartite(const vector<vector<int>> &adj) {
    vector<int> colour(adj.size(), -1);
    for (size_t s = 0; s < adj.size(); s++) {
        if (colour[s] != -1) continue;
        colour[s] = 0;
        queue<int> q;
        q.push((int)s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (colour[v] == -1) {
                    colour[v] = colour[u] ^ 1;
                    q.push(v);
                } else if (colour[v] == colour[u]) {
                    return {};
                }
            }
        }
    }
    return colour;
}

// All-pairs shortest paths. O(n^3) -- only for n <= ~400.
// d[i][j] = INF means unreachable. Handles negative edges (no negative cycle).
const ll INF = numeric_limits<ll>::max() / 4;

void floydWarshall(vector<vector<ll>> &d) {
    size_t n = d.size();
    for (size_t k = 0; k < n; k++)
        for (size_t i = 0; i < n; i++)
            if (d[i][k] < INF)
                for (size_t j = 0; j < n; j++)
                    if (d[k][j] < INF) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

// Minimum spanning tree weight (Kruskal). Returns -1 if the graph is not
// connected. Edges are {weight, u, v}.
ll kruskal(int n, vector<array<ll, 3>> edges) {
    sort(edges.begin(), edges.end());
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);
    function<int(int)> find = [&](int x) {
        while (parent[x] != x) x = parent[x] = parent[parent[x]];
        return x;
    };

    ll total = 0;
    int used = 0;
    for (auto [w, u, v] : edges) {
        int a = find((int)u), b = find((int)v);
        if (a == b) continue;
        parent[a] = b;
        total += w;
        used++;
    }
    return used == n - 1 ? total : -1;
}

#ifdef TEST_GRAPHS2
int main() {
    // DAG: 0->1->3, 0->2->3
    vector<vector<int>> dag(4);
    dag[0] = {1, 2};
    dag[1] = {3};
    dag[2] = {3};
    auto order = toposort(dag);
    assert(order.size() == 4);
    assert(order[0] == 0 && order[3] == 3);

    // Add a cycle 3->0 and it must report failure.
    dag[3].push_back(0);
    assert(toposort(dag).empty());

    // Undirected: path has no cycle, triangle does.
    vector<vector<int>> path(3);
    auto addU = [](vector<vector<int>> &g, int a, int b) {
        g[a].push_back(b);
        g[b].push_back(a);
    };
    addU(path, 0, 1);
    addU(path, 1, 2);
    assert(!hasCycleUndirected(path));
    addU(path, 2, 0);
    assert(hasCycleUndirected(path));

    // Even cycle is bipartite, odd cycle is not.
    vector<vector<int>> even(4);
    addU(even, 0, 1); addU(even, 1, 2); addU(even, 2, 3); addU(even, 3, 0);
    auto col = bipartite(even);
    assert(!col.empty() && col[0] == col[2] && col[0] != col[1]);

    vector<vector<int>> odd(3);
    addU(odd, 0, 1); addU(odd, 1, 2); addU(odd, 2, 0);
    assert(bipartite(odd).empty());

    // Floyd-Warshall on 0->1 (1), 1->2 (2), 0->2 (5); node 3 isolated.
    vector<vector<ll>> d(4, vector<ll>(4, INF));
    for (int i = 0; i < 4; i++) d[i][i] = 0;
    d[0][1] = 1; d[1][2] = 2; d[0][2] = 5;
    floydWarshall(d);
    assert(d[0][2] == 3);   // via 1
    assert(d[0][3] == INF); // unreachable
    assert(d[2][0] == INF); // directed

    // MST: triangle with weights 1,2,3 -> 1+2 = 3
    assert(kruskal(3, {{1, 0, 1}, {2, 1, 2}, {3, 0, 2}}) == 3);
    assert(kruskal(3, {{1, 0, 1}}) == -1);  // disconnected
    assert(kruskal(1, {}) == 0);            // single node
    puts("graphs2 ok");
}
#endif
