/**Gives Area of Union of Rectangles**/

struct rectangle {
    ll xl, yl, xh, yh;
};

long long rectangle_area(vector<rectangle> rs) {
    vector<ll> ys; // coordinate compression
    for (ll i = 0; i < rs.size(); ++i) {
        ys.push_back(rs[i].yl);
        ys.push_back(rs[i].yh);
    }

    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    ll n = ys.size(); // measure tree
    vector<ll> C(8 * n), A(8 * n);

    function<void (ll, ll, ll, ll, ll, ll)> aux =
    [&](ll a, ll b, ll c, ll l, ll r, ll k) {
        if ((a = max(a, l)) >= (b = min(b, r))) {
            return;
        }
        if (a == l && b == r) {
            C[k] += c;
        }
        else {
            aux(a, b, c, l, (l + r) / 2, 2 * k + 1);
            aux(a, b, c, (l + r) / 2, r, 2 * k + 2);
        }
        if (C[k]) {
            A[k] = ys[r] - ys[l];
        }
        else {
            A[k] = A[2 * k + 1] + A[2 * k + 2];
        }
    };

    struct event {
        ll x, l, h, c;
    }; // plane sweep

    vector<event> es;
    for (auto r : rs) {
        ll l = distance(ys.begin(), lower_bound(ys.begin(), ys.end(), r.yl));
        ll h = distance(ys.begin(), lower_bound(ys.begin(), ys.end(), r.yh));
        es.push_back({r.xl, l, h, +1});
        es.push_back({r.xh, l, h, -1});
    }

    sort(es.begin(), es.end(), [](event a, event b) {
        return a.x != b.x ? a.x < b.x : a.c > b.c;
    });

    long long area = 0, prev = 0;
    for (auto &e : es) {
        area += (e.x - prev) * A[0];
        prev = e.x;
        aux(e.l, e.h, e.c, 0, n, 0);
    }
    return area;
}

bool intersect(rectangle A, rectangle B, rectangle &out) {
    out.xl = max(A.xl, B.xl);
    out.yl = max(A.yl, B.yl);
    out.xh = min(A.xh, B.xh);
    out.yh = min(A.yh, B.yh);
    if(out.xl > out.xh || out.yl > out.yh)
        return false;
    return true;
}
