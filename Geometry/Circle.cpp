struct Circle {
    Point center;
    LD r;
    Circle(LD x, LD y, LD rad) {
        center = Point(x, y);
        r = rad;
    }
    Circle(const Point& a, LD rad) : center(a), r(rad) {}

    LD Area() const {
        return kPi * Sq(r);
    }
    LD Perimeter() const {
        return 2 * kPi * r;
    }
    LD Diameter() const {
        return 2 * r;
    }
    Point RotateRightMost(LD ang) const {
        return center + Point{r * cos(ang), r * sin(ang)};
    }
    bool operator==(const Circle& c) const {
        return center == c.center && abs(r - c.r) < kEps;
    }

    //Returns Intersection of Circle and a line
    int CircleLine(Line l, pair<Point,Point> &out) {
        LD h2 = r * r - l.SqDist(center);
        if(h2 < 0) {
            return 0;
        }
        Point P = l.project(center);
        Point h = l.v * sqrt(h2) / (l.v.Norm());
        out = {P - h, P + h};
        return 1 + (h2 > 0);
    }

    //Circle-Circle Intersections
    int CircleCircle(Circle C, pair<Point,Point> &out) {
        Point d = C.center - center;
        LD d2 = d.SqNorm();
        if(d.IsZero()) {
            assert(C.r != r);
            return 0;
        }
        LD pd = (d2 + r * r - C.r * C.r) / 2.0;
        LD h2 = r * r - pd * pd / d2;
        if(h2 < 0) {
            return 0;
        }
        Point p = center + d * pd / d2;
        Point h = d.perp() * sqrt(h2 / d2);
        out = {p - h, p + h};
        return 1 + (h2 > 0);
    }

    //Tangents
    int Tangents(Circle C, bool inner, vector<pair<Point,Point>> &out) {
        if(inner) {
            C.r = -C.r;
        }
        Point d = C.center - center;
        LD dr = r - C.r, d2 = d.SqNorm(), h2 = d2 - dr * dr;
        if(d2 == 0 || h2 < 0) {
            assert(h2 != 0);
            return 0;
        }
        for(int sign : {-1, 1}) {
            Point v = (d * dr + d.perp() * sqrt(h2) * sign) / d2;
            out.push_back({center + v * r, C.center + v * C.r});
        }
        return 1 + (h2 > 0);
    }
};
