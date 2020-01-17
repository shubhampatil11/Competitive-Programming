struct Line {
    Point v; LD c;

    // From points P and Q
    Line(Point p, Point q): v(q - p), c(v.CrossProd(p)) {}
    // From equation ax + by = c
    Line(LD a, LD b, LD c): v({b, -a}), c(c) {}
    // From direction vector v and offset c
    Line(Point v, LD c): v(v), c(c) {}

    /*
    Left side of vector v -> 1
    Right side of vector v -> -1;
    On the line -> 0
    */
    int side(Point p) {
        LD x = v.CrossProd(p) - c;
        if(x < 0) {
            return -1;
        }
        if(x > 0) {
            return 1;
        }
        return 0;
    }
    //Distance of Point p from the line
    LD Dist(Point p) {
        LD x = v.CrossProd(p) - c;
        return (abs(x) / v.Norm());
    }
    LD SqDist(Point p) {
        LD x = v.CrossProd(p) - c;
        return (Sq(x) / v.SqNorm());
    }
    //Perpendicular to Line through Point p
    Line PerpThrough(Point P) {
        return Line(P, P + v.perp());
    }
    //Sort Points on the line in the direction of v
    void SortPoints(vector<Point> &P) {
        sort(P.begin(), P.end(), [&](Point A, Point B) {
            return v.DotProd(A) < v.DotProd(B);
        });
    }
    //Translating a line
    Line translate(Point t) {
        return Line(v, c + v.CrossProd(t));
    }
    Line ShiftLeft(LD dist) {
        return Line(v, c + dist * v.Norm());
    }
    Line ShiftRight(LD dist) {
        return Line(v, c - dist * v.Norm());
    }
    //Projection of Point p on line
    Point project(Point p) {
        LD x = v.CrossProd(p) - c;
        return p - v.perp() * x / v.SqNorm();
    }
    //Reflection of Point p on line
    Point reflect(Point p) {
        LD x = v.CrossProd(p) - c;
        return p - v.perp() * x * 2.0 / v.SqNorm();
    }
    //Compare Projection
    bool CompareProj(Point A, Point B) {
        return v.DotProd(A) < v.DotProd(B);
    }
};
