struct LineUtils {
    static bool isParallel(Line l1, Line l2) {
        if(l1.v == l2.v) {
            return true;
        }
        LD d = l1.v.CrossProd(l2.v);
        if(abs(d) < kEps) {
            return true;
        }
        return false;
    }
    static bool intersection(Line l1, Line l2, Point &out) {
        if(isParallel(l1, l2)) {
            return false;
        }
        LD d = l1.v.CrossProd(l2.v);
        out = (l2.v * l1.c - l1.v * l2.c) / d;
        return true;
    }
    static Line AngleBisector(Line l1, Line l2, bool interior) {
        assert(!isParallel(l1, l2));
        LD sign = interior ? 1 : -1;
        Point v = l1.v.Normalize() + l2.v.Normalize() * sign;
        LD c = l1.c / l1.v.Norm() + sign * l2.c / l2.v.Norm();
        return Line(v, c);
    }
};
LineUtils LU;
