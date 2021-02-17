#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

// returns the vector between two points, aka the difference between coordinates
Point vec(const Point &a, const Point &b) {
    return {b.x - a.x, b.y - a.y};
}

// a 2D cross product isn't a real mathematical concept, but competitive programmers often define this as
// the signed magnitude of the cross product in 3D space, assuming vectors a and b have a z-value of 0
// this is useful because depending on the order (cross(a, b) vs cross(b, a)), the 3D vector would point
// up or down, as designated by the sign of the value this function returns
// if the magnitude is 0, it means a and b are parallel vectors
long long cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

// the current x-value that we are at in the linesweep
long long curX;

struct Segment {
    int idx;    // the index in the order of input
    Point a, b; // a segment is defined by two points

    // this comparator sorts the segments based on y-coordinate when evaluated at curX
    // note that if we directly compute the y-coordinate, we'd have to use doubles and introduce risk of precision error
    // the code below rearranges the equation to do math without doubles
    // curY = (b.y - a.y) * (curX - a.x) / (b.x - a.x) + a.y
    bool operator < (const Segment &other) const {
        if (a.x == b.x) {
            if (other.a.x == other.b.x)
                return tie(a.y, idx) < tie(other.a.y, other.idx);
            long long l = a.y * (other.b.x - other.a.x), r = (other.b.y - other.a.y) * (curX - other.a.x) + other.b.y * (other.b.x - other.a.x);
            if (l == r)
                return idx < other.idx;
            return other.a.x < other.b.x ? l < r : l > r;
        } else if (other.a.x == other.b.x) {
            assert(a.x != b.x);
            long long l = (b.y - a.y) * (curX - a.x) + a.y * (b.x - a.x), r = other.a.y * (b.x - a.x);
            if (l == r)
                return idx < other.idx;
            return a.x < b.x ? l < r : l > r;
        } else {
            // I use 128-bit integers here because values could be up to an order of magnitude of 10^27
            // 64-bit is sufficient for computations in the other two if-clauses as values there only reach order of magnitude of 10^18
            __int128_t l = (__int128_t) (b.y - a.y) * (curX - a.x) * (other.b.x - other.a.x) + (__int128_t) a.y * (b.x - a.x) * (other.b.x - other.a.x);
            __int128_t r = (__int128_t) (other.b.y - other.a.y) * (curX - other.a.x) * (b.x - a.x) + (__int128_t) other.a.y * (b.x - a.x) * (other.b.x - other.a.x);
            if (l == r)
                return idx < other.idx;
            return (a.x < b.x) == (other.a.x < other.b.x) ? l < r : l > r;
        }
    }
};

// checks if a point lies on a segment
// prerequisite: the point lies on the infinite line formed by the endpoints of the segment
bool onSegment(const Segment &s, const Point &p) {
    return min(s.a.x, s.b.x) <= p.x && p.x <= max(s.a.x, s.b.x) && min(s.a.y, s.b.y) <= p.y && p.y <= max(s.a.y, s.b.y);
}

// checks if two line segments intersect
// this method of testing intersection uses vector cross products, involves no doubles, so is immune to precision error
// https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/ - teaches it without cross products, instead
// introduces a concept called "orientation" that is functionally equivalent to the sign of a cross product
bool intersect(const Segment &s, const Segment &t) {
    long long w = cross(vec(s.a, s.b), vec(t.a, s.b)), x = cross(vec(s.a, s.b), vec(t.b, s.b));
    long long y = cross(vec(t.a, t.b), vec(s.a, t.b)), z = cross(vec(t.a, t.b), vec(s.b, t.b));
    return ((w > 0) != (x > 0) && (y > 0) != (z > 0)) ||
            (w == 0 && onSegment(s, t.a)) ||
            (x == 0 && onSegment(s, t.b)) ||
            (y == 0 && onSegment(t, s.a)) ||
            (z == 0 && onSegment(t, s.b));
}

int main() {
    freopen("cowjump.in", "r", stdin);
    freopen("cowjump.out", "w", stdout);

    // handle input
    int n;
    cin >> n;
    vector<Segment> s(n);
    vector<pair<long long, int>> v;
    for (int i=0; i<n; i++) {
        cin >> s[i].a.x >> s[i].a.y >> s[i].b.x >> s[i].b.y;
        s[i].idx = i;
        v.emplace_back(min(s[i].a.x, s[i].b.x), i);     // left endpoint of segment
        v.emplace_back(max(s[i].a.x, s[i].b.x), i + n); // right endpoint of segment
        // i + n is used for the right endpoint so that for the same x-value,
        // all insertion events occur before all deletion events
    }

    sort(v.begin(), v.end());
    int retA = -1, retB = -1;
    set<Segment> st;
    for (auto [x, i] : v) {
        curX = x;
        if (i >= n) {
            // delete a segment from the set
            i -= n;
            auto it = st.find(s[i]);
            // now check if the two segments on either side of the deleted one intersect
            if (it != st.begin() && next(it) != st.end() && intersect(*prev(it), *next(it))) {
                retA = min(prev(it)->idx, next(it)->idx);
                retB = max(prev(it)->idx, next(it)->idx);
                break;
            }
            st.erase(it);
        } else {
            // insert a segment into the set
            auto it = st.insert(s[i]).first;
            // check for intersection with the two neighboring segments in the set
            if (it != st.begin() && intersect(*it, *prev(it))) {
                retA = min(it->idx, prev(it)->idx);
                retB = max(it->idx, prev(it)->idx);
                break;
            }
            if (next(it) != st.end() && intersect(*it, *next(it))) {
                retA = min(it->idx, next(it)->idx);
                retB = max(it->idx, next(it)->idx);
                break;
            }
        }
    }
    assert(retA != -1);

    // to determine which of the two segments is our answer, check which one intersects with more segments
    // in case of a tie, removing either is fine, and problem wants us to remove the one with lower index
    int cnt = 0;
    for (int i=0; i<n; i++)
        if (i != retB && intersect(s[i], s[retB]))
            cnt++;
    cout << (cnt > 1 ? retB + 1 : retA + 1) << "\n";

    return 0;
}
