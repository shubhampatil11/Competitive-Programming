struct Polygon {
    vector<Point> pts;
    Polygon(vector<Point> pts_) : pts(pts_) {}
    Polygon() : Polygon(vector<Point>()) {}

    void Add(Point p) {
        pts.push_back(p);
    }

    // positive for counterclockwise
    LD Area() {
        LD area = 0;
        int SZ = (int)pts.size();
        for(int i = 0; i < SZ; i++) {
            area += pts[i].CrossProd(pts[(i + 1) % SZ]);
        }
        area /= 2;
        return area;
    }

    void OrientCounterclockwise() {
        if (Area() < 0)
            reverse(pts.begin(), pts.end());
    }

    int next(int a) {
        if (a + 1 < (int)pts.size()) {
            return a + 1;
        }
        return 0;
    }

    //true if P is at least as high as A
    bool half(Point A, Point P) {
        return P.y >= A.y;
    }
    //check if PQ crosses ray from A
    bool CrossesRay(Point A, Point P, Point Q) {
        return (half(A, Q) - half(A, P)) * PU.orient(A, P, Q) > 0;
    }

    //If strict, then returns false when A is on the boundary
    bool InPolygon(Point A, bool strict = true) {
        int NumCrossings = 0;
        int n = (int)pts.size();
        for(int i = 0; i < n; i++) {
            if(PU.OnSegment(pts[i], pts[(i + 1) % n], A)) {
                return !strict;
            }
            NumCrossings += CrossesRay(A, pts[i], pts[(i + 1) % n]);
        }
        return (NumCrossings & 1);
    }

    /*
    For square 34
               12
    holds one_way_hull = {{1,3,4},{1,2,4}}
    Resulting polygon is counterclockwise {1, 2, 4, 3}
    */
    vector<vector<Point>> MakeConvexHull() {
        vector<vector<Point>> one_way_hull(2);

        sort(pts.begin(), pts.end(), Point::LexCmp);

        for (int dir = -1; dir <= 1; dir += 2) {
            int hull_num = (dir + 1) / 2;
            auto& H = one_way_hull[hull_num];
            one_way_hull[hull_num].push_back(pts[0]);
            if ((int)pts.size() > 1)
                H.push_back(pts[1]);
            for (int i = 2; i < (int)pts.size(); i++) {
                while ((int)H.size() >= 2 &&
                        dir * (pts[i] - H[(int)H.size() - 2]).CrossProd(H.back() - H[(int)H.size() - 2]) > -kEps)
                    H.pop_back();
                H.push_back(pts[i]);
            }
            if ((int)H.size() > 1 && (H[0] - H.back()).IsZero())
                H.pop_back();
        }

        pts.clear();

        for (auto p : one_way_hull[1])
            pts.push_back(p);

        for (int i = (int)one_way_hull[0].size() - 2; i >= 1; i--)
            pts.push_back(one_way_hull[0][i]);

        return one_way_hull;
    }

    //Furthest pair of points in a polygon
    pair<int, int> FurthestPair() {
        MakeConvexHull();
        OrientCounterclockwise();
        int furth = 1;
        pair<int, int> best_pair = make_pair(0, 0);
        double best_dis = 0;
        for (int i = 0; i < (int)pts.size(); i++) {
            Point side = pts[next(i)] - pts[i];
            while (side.CrossProd(pts[furth] - pts[i]) < side.CrossProd(pts[next(furth)] - pts[i]))
                furth = next(furth);
            vector<int> vec{i, next(i)};
            for (auto ind : vec) {
                if (pts[ind].Dist(pts[furth]) > best_dis) {
                    best_pair = make_pair(ind, furth);
                    best_dis = pts[ind].Dist(pts[furth]);
                }
            }
            //cerr << "Furthest from: " << pts[i] << "-" << pts[next(i)] << " is " << pts[furth] << endl;
        }
        return best_pair;
    }
};
