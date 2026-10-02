// Union-Find / DSU with union by size + path compression.
// Near O(1) amortised per operation.
//
// WHY THIS FILE EXISTS: KACTL deliberately omits plain Union-Find
// (content/data-structures/chapter.tex has `% \kactlimport{UnionFind.h}`
// commented out, shipping only UnionFindRollback). NCPC's easy/medium graph
// problems lean on exactly this. Do not go to the contest without it.
//
// Typical uses: connected components, cycle detection in an undirected graph,
// Kruskal's MST, "are a and b in the same group" queries.
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, size_;
    int components;

    explicit DSU(int n) : parent(n), size_(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];  // path halving
            x = parent[x];
        }
        return x;
    }

    bool same(int a, int b) { return find(a) == find(b); }

    // Returns false if a and b were already connected.
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size_[a] < size_[b]) swap(a, b);
        parent[b] = a;
        size_[a] += size_[b];
        components--;
        return true;
    }

    int compSize(int x) { return size_[find(x)]; }
};

#ifdef TEST_DSU
int main() {
    DSU d(6);
    assert(d.components == 6);
    assert(d.unite(0, 1));
    assert(d.unite(1, 2));
    assert(!d.unite(0, 2));          // already connected
    assert(d.same(0, 2));
    assert(!d.same(0, 3));
    assert(d.compSize(0) == 3);
    assert(d.components == 4);
    d.unite(3, 4);
    d.unite(4, 5);
    d.unite(2, 5);
    assert(d.components == 1);
    assert(d.compSize(0) == 6);
    puts("DSU ok");
}
#endif
