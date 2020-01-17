/**
Returns Convex Hull Minkowski Sum of two convex polygon.
TESTED: https://www.codechef.com/viewsolution/27222902
**/

Polygon MinkowskiSum(const Polygon &P1, const Polygon &P2) {
    vector<Point> poly1 = P1.pts;
    vector<Point> poly2 = P2.pts;
    int mem = 0;
    vector<Point> dirs;
    for (int i = 0; i < (int)poly1.size(); ++i) {
        int next = (i + 1) % (int)poly1.size();
        dirs.push_back(poly1[next] - poly1[i]);
        if (poly1[i].y > poly1[mem].y || (poly1[i].y == poly1[mem].y && poly1[i].x > poly1[mem].x)) {
            mem = i;
        }
    }
    Point shift = poly1[mem];
    mem = 0;
    for (int i = 0; i < (int)poly2.size(); ++i) {
        int next = (i + 1) % (int)poly2.size();
        dirs.push_back(poly2[next] - poly2[i]);
        if (poly2[i].y > poly2[mem].y || (poly2[i].y == poly2[mem].y && poly2[i].x > poly2[mem].x)) {
            mem = i;
        }
    }
    shift = shift + poly2[mem];
    sort(dirs.begin(), dirs.end(), [&](auto & p1, auto & p2) {
        bool fl1 = p1.y > 0 || (p1.y == 0 && p1.x > 0);
        bool fl2 = p2.y > 0 || (p2.y == 0 && p2.x > 0);
        if (fl1 != fl2) {
            return fl1 < fl2;
        }
        return p1.CrossProd(p2) > 0;
    });
    vector<Point> ret;
    for (auto d : dirs) {
        ret.push_back(shift);
        shift = shift + d;
    }
    return Polygon(ret);
}
