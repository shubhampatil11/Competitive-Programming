struct PointUtils {
    static LD Angle(Point P, Point Q, Point R) { // angle PQR, it can be negative
        LD ang2 = (P - Q).Angle();
        LD ang1 = (R - Q).Angle();
        LD ans = ang1 - ang2;
        return ans;
    }
    /*
    A, B, C Orientation
    Counter clockwise -> 1;
    Clockwise -> -1;
    Collinear -> 0;
    */
    static int orient(Point A, Point B, Point C) {
        LD X = (B - A).CrossProd(C - A);
        if(X < 0) {
            return -1;
        }
        if(X < kEps) {
            return 0;
        }
        return 1;
    }
    //Returns magnitude of orientation A, B, C
    static LD OrientMag(Point A, Point B, Point C) {
        return (B - A).CrossProd(C - A);
    }
    //Find whether X is in the angle CAB
    static bool inAngle(Point A, Point B, Point C, Point X) {
        assert(orient(A, B, C) != 0);
        if(orient(A, B, C) < 0) {
            swap(B, C);
        }
        return orient(A, B, X) >=0 && orient(A, C, X) <= 0;
    }
    //Check whether given points form convex polygon or not
    static bool isConvex(vector<Point> P) {
        bool hasPos = false, hasNeg = false;
        int n = (int)P.size();
        for(int i = 0; i < n; i++) {
            int x = orient(P[i], P[(i + 1) % n], P[(i + 2) % n]);
            if(x > 0) {
                hasPos = true;
            }
            if(x < 0) {
                hasNeg =true;
            }
        }
        return !(hasPos && hasNeg);
    }
    static bool half(Point A) {
        assert(A.x != 0 && A.y != 0);
        return (A.y > 0 || (A.y == 0 && A.x < 0));
    }
    //Polar sort around Origin in Counter Clockwise direction
    static void PolarSort(vector<Point> &v) {
        sort(v.begin(), v.end(), [](Point A, Point B) {
            if(half(A) == half(B)) {
                return (A.CrossProd(B) > 0);
            }
            return half(A);
        });
    }
    //Polar sort Around Point O in Counter Clockwise direction
    static void PolarSortAround(vector<Point> &v, Point O) {
        sort(v.begin(), v.end(), [&](Point A, Point B) {
            if(half(A - O) == half(B - O)) {
                return ((A - O).CrossProd(B - O) > 0);
            }
            return half(A - O);
        });
    }
    //Check whether P is inside a disk considering A & B as diameter
    static bool InDisk(Point A, Point B, Point P) {
        return ((A - P).DotProd(B - P) <= 0);
    }
    //Check whether P is on the segment A & B
    static bool OnSegment(Point A, Point B, Point P) {
        return orient(A, B, P) == 0 && InDisk(A, B, P);
    }
    //Return Proper Intersection of Segments A,B & C,D
    static bool ProperIntersect(Point A, Point B, Point C, Point D, Point &out) {
        int oa = orient(C, D, A);
        int ob = orient(C, D, B);
        int oc = orient(A, B, C);
        int od = orient(A, B, D);
        if(oa * ob < 0 && oc * od < 0) {
            out = (A * OrientMag(C, D, B) - B * OrientMag(C, D, A)) / (OrientMag(C, D, B) - OrientMag(C, D, A));
            return true;
        }
        return false;
    }
    //Returns Intersection Points of Segments A,B & C,D
    static vector<Point> IntersectSeg(Point A, Point B, Point C, Point D) {
        Point out;
        if(ProperIntersect(A, B, C, D, out)) {
            return {out};
        }
        vector<Point> s;
        if(OnSegment(A, B, C)) {
            s.push_back(C);
        }
        if(OnSegment(A, B, D)) {
            s.push_back(D);
        }
        if(OnSegment(C, D, A)) {
            s.push_back(A);
        }
        if(OnSegment(C, D, B)) {
            s.push_back(B);
        }
        return s;
    }
    //Segment Point Distance
    static LD SegPoint(Point A, Point B, Point P) {
        if(!(A == B)) {
            Line l(A, B);
            if(l.CompareProj(A, P) && l.CompareProj(P, B)) {
                return l.Dist(P);
            }
        }
        return min((P - A).Norm(), (P - B).Norm());
    }
    //Segment-Segment Distance
    static LD SegSegDist(Point A, Point B, Point C, Point D) {
        Point temp;
        if(ProperIntersect(A, B, C, D, temp)) {
            return 0;
        }
        return min({SegPoint(A, B, C), SegPoint(A, B, D), SegPoint(C, D, A), SegPoint(C, D, B)});
    }
    /*
    **NOT TESTED**
    Distance between two rays
    A -> Initial Point a -> Direction
    B -> Initial Point b -> Direction
    D is the point on a and E is the point on b
    Distance is distance between D and E
    */
    static LD RaysDist(Point A, Point a, Point B, Point b) {
        Point c = B - A;
        LD num1 = -(a.DotProd(c) * b.DotProd(c)) + (a.DotProd(c) * b.DotProd(b));
        LD den1 = (a.DotProd(a) * b.DotProd(b)) - (a.DotProd(b) * a.DotProd(b));

        LD num2 = (a.DotProd(b) * a.DotProd(c)) - (b.DotProd(c) * a.DotProd(a));
        LD den2 = (a.DotProd(a) * b.DotProd(b)) - (a.DotProd(b) * a.DotProd(b));

        Point D = A + a * num1 / den1;
        Point E = B + b * num2 / den2;

        return D.Dist(E);
    }
};
PointUtils PU;
