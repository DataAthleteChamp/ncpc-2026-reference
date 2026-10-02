// Computational geometry. NCPC uses geometry heavily for a contest of its
// size (9 of 57 problems, 2021-2025), and convex hull alone appeared three
// times in five years.
//
// TRAPS:
//   * Prefer INTEGER coordinates. cross() on long long is exact; doubles
//     introduce comparison bugs you will not find under time pressure.
//   * cross(a,b,c) > 0 means c is LEFT of the directed line a->b.
//   * Polygon area via the shoelace formula is TWICE the area; it is even,
//     so halving is exact -- but keep it doubled while summing.
//   * Collinear points: decide explicitly whether the hull should keep them.
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct P {
    ll x, y;
    P operator-(const P &o) const { return {x - o.x, y - o.y}; }
    bool operator<(const P &o) const { return tie(x, y) < tie(o.x, o.y); }
    bool operator==(const P &o) const { return x == o.x && y == o.y; }
};

// Cross product of OA x OB. Sign gives orientation:
//   > 0  counter-clockwise (b is left of o->a)
//   = 0  collinear
//   < 0  clockwise
ll cross(const P &o, const P &a, const P &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

ll dot(const P &o, const P &a, const P &b) {
    return (a.x - o.x) * (b.x - o.x) + (a.y - o.y) * (b.y - o.y);
}

ll dist2(const P &a, const P &b) {
    ll dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// TWICE the signed area of a polygon (shoelace). Positive if the vertices
// are counter-clockwise. Doubling keeps it exact in integers.
ll area2(const vector<P> &poly) {
    ll s = 0;
    for (size_t i = 0, n = poly.size(); i < n; i++) {
        const P &a = poly[i], &b = poly[(i + 1) % n];
        s += a.x * b.y - b.x * a.y;
    }
    return s;
}

// Convex hull, counter-clockwise, Andrew's monotone chain. O(n log n).
// Collinear points on the hull edges are EXCLUDED (strict `<= 0`).
// To keep them, change to `< 0`.
vector<P> convexHull(vector<P> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = (int)pts.size();
    if (n < 3) return pts;

    vector<P> h(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(h[k - 2], h[k - 1], pts[i]) <= 0) k--;
        h[k++] = pts[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && cross(h[k - 2], h[k - 1], pts[i]) <= 0) k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1);
    return h;
}

// Is point q inside (or on the border of) a polygon? Ray casting, O(n).
// Works for any simple polygon, convex or not.
bool pointInPolygon(const vector<P> &poly, const P &q) {
    bool inside = false;
    for (size_t i = 0, n = poly.size(), j = n - 1; i < n; j = i++) {
        const P &a = poly[i], &b = poly[j];
        // On the boundary?
        if (cross(a, b, q) == 0 && dot(q, a, b) <= 0) return true;
        if ((a.y > q.y) != (b.y > q.y)) {
            // Does the ray to the right cross edge a-b?
            ll t = cross(a, b, q);
            if ((b.y > a.y) ? t > 0 : t < 0) inside = !inside;
        }
    }
    return inside;
}

// Do segments p1p2 and p3p4 intersect (including touching)?
bool segmentsIntersect(const P &p1, const P &p2, const P &p3, const P &p4) {
    auto sgn = [](ll v) { return (v > 0) - (v < 0); };
    int d1 = sgn(cross(p3, p4, p1)), d2 = sgn(cross(p3, p4, p2));
    int d3 = sgn(cross(p1, p2, p3)), d4 = sgn(cross(p1, p2, p4));
    if (d1 * d2 < 0 && d3 * d4 < 0) return true;
    // Collinear touching cases.
    auto onSeg = [](const P &a, const P &b, const P &c) {
        return cross(a, b, c) == 0 && min(a.x, b.x) <= c.x && c.x <= max(a.x, b.x) &&
               min(a.y, b.y) <= c.y && c.y <= max(a.y, b.y);
    };
    return onSeg(p3, p4, p1) || onSeg(p3, p4, p2) || onSeg(p1, p2, p3) ||
           onSeg(p1, p2, p4);
}

#ifdef TEST_GEOMETRY
int main() {
    P o{0, 0}, a{1, 0}, b{0, 1};
    assert(cross(o, a, b) > 0);   // counter-clockwise
    assert(cross(o, b, a) < 0);   // clockwise
    assert(cross(o, a, P{2, 0}) == 0);  // collinear
    assert(dist2(o, P{3, 4}) == 25);

    // Unit square, counter-clockwise: area 1 -> area2 == 2
    vector<P> sq = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    assert(area2(sq) == 2);
    reverse(sq.begin(), sq.end());
    assert(area2(sq) == -2);      // clockwise flips the sign
    reverse(sq.begin(), sq.end());

    // 10x10 square with an interior point: hull must drop the interior one.
    vector<P> pts = {{0, 0}, {10, 0}, {10, 10}, {0, 10}, {5, 5}};
    auto h = convexHull(pts);
    assert(h.size() == 4);
    assert(area2(h) == 200);

    // Collinear points along an edge are excluded.
    auto h2 = convexHull({{0, 0}, {1, 0}, {2, 0}, {2, 2}, {0, 2}});
    assert(h2.size() == 4);

    // Degenerate inputs must not crash.
    assert(convexHull({{1, 1}}).size() == 1);
    assert(convexHull({{0, 0}, {1, 1}}).size() == 2);
    assert(convexHull({{3, 3}, {3, 3}}).size() == 1);  // duplicates removed

    vector<P> poly = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    assert(pointInPolygon(poly, {2, 2}));   // inside
    assert(pointInPolygon(poly, {0, 0}));   // vertex
    assert(pointInPolygon(poly, {2, 0}));   // on an edge
    assert(!pointInPolygon(poly, {5, 2}));  // outside
    assert(!pointInPolygon(poly, {-1, -1}));

    assert(segmentsIntersect({0, 0}, {2, 2}, {0, 2}, {2, 0}));   // crossing
    assert(segmentsIntersect({0, 0}, {2, 0}, {1, 0}, {3, 0}));   // overlapping
    assert(segmentsIntersect({0, 0}, {1, 1}, {1, 1}, {2, 2}));   // touching
    assert(!segmentsIntersect({0, 0}, {1, 1}, {2, 2}, {3, 3}));  // collinear apart
    assert(!segmentsIntersect({0, 0}, {1, 0}, {0, 1}, {1, 1}));  // parallel

    // Overflow guard: coordinates near 1e9 make cross() reach ~1e18,
    // which fits long long but would destroy int.
    P big1{0, 0}, big2{1'000'000'000LL, 0}, big3{0, 1'000'000'000LL};
    assert(cross(big1, big2, big3) == 1'000'000'000'000'000'000LL);
    puts("geometry ok");
}
#endif
