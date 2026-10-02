// BFS and DFS on an unweighted graph, plus grid BFS. O(n + m).
//
// WHY THIS FILE EXISTS: KACTL names BFS/DFS only in its "Techniques" appendix
// and never implements them. These are the workhorses of NCPC's easy graph
// problems.
//
// TRAPS:
//   * Recursive DFS stack-overflows around ~1e5 depth on a path graph.
//     Use the iterative version for large inputs.
//   * BFS gives shortest path ONLY when every edge has the same weight.
//   * In grids, bounds-check BEFORE indexing.
#include <bits/stdc++.h>
using namespace std;

// Shortest distances in edges from src; -1 if unreachable.
vector<int> bfs(const vector<vector<int>> &adj, int src) {
    vector<int> dist(adj.size(), -1);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return dist;
}

// Iterative DFS — safe at any depth. Returns visit order.
vector<int> dfs(const vector<vector<int>> &adj, int src) {
    vector<char> seen(adj.size(), 0);
    vector<int> order, st{src};
    seen[src] = 1;
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        order.push_back(u);
        for (int v : adj[u])
            if (!seen[v]) {
                seen[v] = 1;
                st.push_back(v);
            }
    }
    return order;
}

// Number of connected components.
int countComponents(const vector<vector<int>> &adj) {
    vector<char> seen(adj.size(), 0);
    int c = 0;
    for (size_t i = 0; i < adj.size(); i++)
        if (!seen[i]) {
            c++;
            for (int u : dfs(adj, (int)i)) seen[u] = 1;
        }
    return c;
}

// Grid BFS from (sr,sc); '#' is a wall. Returns -1 for unreachable cells.
vector<vector<int>> gridBFS(const vector<string> &g, int sr, int sc) {
    int R = (int)g.size(), C = (int)g[0].size();
    vector<vector<int>> dist(R, vector<int>(C, -1));
    const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    queue<pair<int, int>> q;
    dist[sr][sc] = 0;
    q.emplace(sr, sc);
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;  // bounds FIRST
            if (g[nr][nc] == '#' || dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.emplace(nr, nc);
        }
    }
    return dist;
}

#ifdef TEST_TRAVERSAL
int main() {
    // 0-1-2   3-4   5
    vector<vector<int>> adj(6);
    auto add = [&](int a, int b) { adj[a].push_back(b); adj[b].push_back(a); };
    add(0, 1); add(1, 2); add(3, 4);

    auto d = bfs(adj, 0);
    assert(d[0] == 0 && d[1] == 1 && d[2] == 2);
    assert(d[3] == -1 && d[5] == -1);
    assert((int)dfs(adj, 0).size() == 3);
    assert(countComponents(adj) == 3);

    // Grid: start top-left, a wall forces a detour, and 'X' is sealed off.
    vector<string> g = {
        "....",
        ".##.",
        ".#X.",
        "...."};
    auto gd = gridBFS(g, 0, 0);
    assert(gd[0][0] == 0);
    assert(gd[0][3] == 3);   // straight along the top row
    assert(gd[3][0] == 3);   // straight down the left column
    assert(gd[2][2] == 6);   // 'X' is reachable, but only the long way around
    assert(gd[1][1] == -1);  // wall
    puts("traversal ok");
}
#endif
