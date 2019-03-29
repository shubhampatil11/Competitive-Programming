const LD kEps = 1e-9;
const LD kPi = 2 * acos(0);
LD Sq(LD x) {
  return x * x;
}
struct Point {
  LD x, y;
  Point() {}
  Point(LD a, LD b) : x(a), y(b) {}
  Point(const Point& a) : x(a.x), y(a.y) {}
  void operator=(const Point& a) { x = a.x; y = a.y; }
  Point operator+(const Point& a) const { Point p(x + a.x, y + a.y); return p; }
  Point operator-(const Point& a) const { Point p(x - a.x, y - a.y); return p; }
  Point operator*(LD a) const { Point p(x * a, y * a); return p; }
  Point operator/(LD a) const { assert(abs(a) > kEps); Point p(x / a, y / a); return p; }
  Point& operator+=(const Point& a) { x += a.x; y += a.y; return *this; }
  Point& operator-=(const Point& a) { x -= a.x; y -= a.y; return *this; }
  Point& operator*=(LD a) { x *= a; y *= a; return *this;}
  Point& operator/=(LD a) { assert(abs(a) > kEps); x /= a; y /= a; return *this; }

  bool IsZero() const {
    return abs(x) < kEps && abs(y) < kEps;
  }
  bool operator==(const Point& a) const {
    return (*this - a).IsZero();
  }
  LD CrossProd(const Point& a) const {
    return x * a.y - y * a.x;
  }
  LD CrossProd(Point a, Point b) const {
    a -= *this;
    b -= *this;
    return a.CrossProd(b);
  }
  LD DotProd(const Point& a) const {
    return x * a.x + y * a.y;
  }
  LD Norm() const {
    return sqrt(Sq(x) + Sq(y));
  }
  void NormalizeSelf() {
    *this /= Norm();
  }
  Point Normalize() {
    Point res(*this);
    res.NormalizeSelf();
    return res;
  }
  LD Dist(const Point& a) const {
    return (*this - a).Norm();
  }
  LD Angle() const {
    return atan2(y, x);
  }
  void RotateSelf(LD angle) {
    LD c = cos(angle);
    LD s = sin(angle);
    LD nx = x * c - y * s;
    LD ny = y * c + x * s;
    y = ny;
    x = nx;
  }
  Point Rotate(LD angle) const {
    Point res(*this);
    res.RotateSelf(angle);
    return res;
  }
  static bool LexCmp(const Point& a, const Point& b) {
    if (abs(a.x - b.x) > kEps) {
      return a.x < b.x;
    }
    return a.y < b.y;
  }
  LD SqNorm() {
    return x * x + y * y;
  }
  friend ostream& operator<<(ostream& out, Point m);
};

ostream& operator<<(ostream& out, Point p) {
  out << "(" << p.x << ", " << p.y << ")";
  return out;
}

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
};

struct Line {
  Point p[2];
  bool is_seg;
  Line(Point a, Point b, bool is_seg_ = false) {
    p[0] = a;
    p[1] = b;
    is_seg = is_seg_;
  }
  Line() {
  }
  Point& operator[](int a) {
    return p[a];
  }
  Point NormalVector() {
    Point perp = p[1] - p[0];
    perp.RotateSelf(kPi / 2);
    perp.NormalizeSelf();
    return perp;
  }

  // (A, B, C) such that A^2 + B^2 = 1, (A, B) > (0, 0)
  vector<LD> LineEqNormLD() { // seems ok
    LD A = p[1].y - p[0].y;
    LD B = p[0].x - p[1].x;
    LD C = -(A * p[0].x + B * p[0].y);
    assert(abs(A * p[1].x + B * p[1].y + C) < kEps);
    LD norm = sqrt(Sq(A) + Sq(B));
    vector<LD> res{A, B, C};
    for (auto& x : res) { x /= norm; }
    if (A < -kEps || (abs(A) < kEps && B < -kEps)) {
      for (auto& x : res) { x *= -1; }
    }
    return res;
  }

  // assumes that coordinates are integers!
  vector<int> LineEqNormInt() { // seems ok
    int A = round(p[1].y - p[0].y);
    int B = round(p[0].x - p[1].x);
    int C = -(A * p[0].x + B * p[0].y);
    int gcd = abs(__gcd(A, __gcd(B, C)));
    vector<int> res{A, B, C};
    for (auto& x : res) { x /= gcd; }
    if (A < 0 || (A == 0 && B < 0)) {
      for (auto& x : res) { x *= -1; }
    }
    return res;
  }
};

struct Utils {
  // 0, 1, 2 or 3 pts. In case of 3 pts it means they are equal
  static vector<Point> InterCircleCircle(Circle a, Circle b) {
    if (a.r + kEps < b.r) {
      swap(a, b);
    }
    if (a == b) {
      return vector<Point>{a.RotateRightMost(0), a.RotateRightMost(2 * kPi / 3),
          a.RotateRightMost(4 * kPi / 3)};
    }
    Point diff = b.center - a.center;
    LD dis = diff.Norm();
    LD ang = diff.Angle();
    LD longest = max(max(a.r, b.r), dis);
    LD per = a.r + b.r + dis;
    if (2 * longest > per + kEps) {
      return vector<Point>();
    }
    if (abs(2 * longest - per) < 2 * kEps) {
      return vector<Point>{a.RotateRightMost(ang)};
    }
    LD ang_dev = acos((Sq(a.r) + Sq(dis) - Sq(b.r)) / (2 * a.r * dis));
    return vector<Point>{a.RotateRightMost(ang - ang_dev), a.RotateRightMost(ang + ang_dev)};
  }

  static vector<Point> InterLineLine(Line& a, Line& b) { // working fine
    Point vec_a = a[1] - a[0];
    Point vec_b1 = b[1] - a[0];
    Point vec_b0 = b[0] - a[0];
    LD tr_area = vec_b1.CrossProd(vec_b0);
    LD quad_area = vec_b1.CrossProd(vec_a) + vec_a.CrossProd(vec_b0);
    if (abs(quad_area) < kEps) { // parallel or coinciding
      if (PtBelongToLine(b, a[0])) {
        return {a[0], a[1]};
      } else {
        return {};
      }
    }
    return {a[0] + vec_a * (tr_area / quad_area)};
  }

  static Point ProjPointToLine(Point p, Line l) { ///Tested
    Point diff = l[1] - l[0];
    return l[0] + diff * (diff.DotProd(p - l[0]) / diff.DotProd(diff));
  }

  static Point ReflectPtWRTLine(Point p, Line l) {
    Point proj = ProjPointToLine(p, l);
    return proj * 2 - p;
  }

  static vector<Point> InterCircleLine(Circle c, Line l) { /// Tested here: http://codeforces.com/gym/100554/submission/10197624
    Point proj = ProjPointToLine(c.center, l);
    LD dis_proj = c.center.Dist(proj);
    if (dis_proj > c.r + kEps) { return vector<Point>(); }
    LD a = sqrt(max((LD)0, Sq(c.r) - Sq(dis_proj)));
    Point dir = l[1] - l[0];
    LD dir_norm = dir.Norm();
    vector<Point> cands{proj + dir * (a / dir_norm), proj - dir * (a / dir_norm)};
    if (cands[0].Dist(cands[1]) < kEps) { return vector<Point>{proj}; }
    return cands;
  }

  static bool PtBelongToLine(Line l, Point p) {
    return abs(l[0].CrossProd(l[1], p)) < kEps;
  }

  static bool PtBelongToSeg(Line l, Point p) { // seems ok
    return abs(p.Dist(l[0]) + p.Dist(l[1]) - l[0].Dist(l[1])) < kEps;
  }

  static vector<Point> InterCircleSeg(Circle c, Line l) { //seems ok
    vector<Point> from_line = InterCircleLine(c, l);
    vector<Point> res;
    for (auto p : from_line) {
      if (PtBelongToSeg(l, p)) { res.PB(p); }
    }
    return res;
  }

  static vector<Point> TangencyPtsToCircle(Circle c, Point p) { // seems ok
    LD d = c.center.Dist(p);
    if (d < c.r - kEps) { return {}; }
    if (d < c.r + kEps) { return {p}; }
    LD from_cent = (p - c.center).Angle();
    LD ang_dev = acos(c.r / d);
    return {c.RotateRightMost(from_cent - ang_dev), c.RotateRightMost(from_cent + ang_dev)};
  }

  // outer and inner tangents tested only locally (however I believe that rigorously)
  static vector<Line> OuterTangents(Circle c1, Circle c2) {
    if (c1 == c2) { return {}; } // is it surely best choice?
    if (c1.r < c2.r) { swap(c1, c2); }
    if (c2.r + c1.center.Dist(c2.center) < c1.r - kEps) { return {}; }
    if (abs(c1.r - c2.r) < kEps) {
      Point diff = c2.center - c1.center;
      Point R = diff.Rotate(kPi / 2) * (c1.r / diff.Norm());
      return {{c1.center + R, c2.center + R}, {c1.center - R, c2.center - R}};
    }
    Point I = c1.center + (c2.center - c1.center) * (c1.r / (c1.r - c2.r));
    if (c2.r + c1.center.Dist(c2.center) < c1.r + kEps) {
      return {{I, I + (c2.center - c1.center).Rotate(kPi / 2)}};
    }
    vector<Point> to1 = TangencyPtsToCircle(c1, I);
    vector<Point> to2 = TangencyPtsToCircle(c2, I);
    vector<Line> res{{to1[0], to2[0]}, {to1[1], to2[1]}};
    assert(Utils::PtBelongToLine(res[0], I));
    assert(Utils::PtBelongToLine(res[1], I));
    return res;
  }

  // unfortunately big part of code is same as in previous function
  // can be joined when putting appropriate signs in few places
  // however those ifs differ a bit hence it may not be good idea
  // to necessarily join them
  static vector<Line> InnerTangents(Circle c1, Circle c2) {
    if (c1 == c2) { return {}; } // this time surely best choice
    if (c1.r < c2.r) { swap(c1, c2); }
    LD d = c1.center.Dist(c2.center);
    if (d < c1.r + c2.r - kEps) { return {}; }
    Point I = c1.center + (c2.center - c1.center) * (c1.r / (c1.r + c2.r));
    if (d < c1.r + c2.r + kEps) {
      return {{I, I + (c2.center - c1.center).Rotate(kPi / 2)}};
    }
    vector<Point> to1 = TangencyPtsToCircle(c1, I);
    vector<Point> to2 = TangencyPtsToCircle(c2, I);
    vector<Line> res{{to1[0], to2[0]}, {to1[1], to2[1]}};
    assert(Utils::PtBelongToLine(res[0], I));
    assert(Utils::PtBelongToLine(res[1], I));
    return res;
  }

  static bool AreParallel(Line l1, Line l2) { // seems ok
    return abs(l1[0].CrossProd(l2[0], l1[1]) - l1[0].CrossProd(l2[1], l1[1])) < kEps;
  }

  // returns a vector of points such that their convex hull is intersection of those segments
  // SZ(res) == 0 => empty intersection, SZ(res) == 1 => intersection is a point, SZ(res) == 2 => intersection is a segment
  static vector<Point> InterSegs(Line l1, Line l2) { // seems ok
    if (!Point::LexCmp(l1[0], l1[1])) { swap(l1[0], l1[1]); }
    if (!Point::LexCmp(l2[0], l2[1])) { swap(l2[0], l2[1]); }
    if (AreParallel(l1, l2)) {
      if (!PtBelongToLine(l1, l2[0])) { return vector<Point>(); }
      vector<Point> ends(2);
      for (int tr = 0; tr < 2; tr++) {
        if (Point::LexCmp(l1[tr], l2[tr]) ^ tr) {
          ends[tr] = l2[tr];
        } else {
          ends[tr] = l1[tr];
        }
      }
      if ((ends[1] - ends[0]).IsZero()) {
        ends.pop_back();
      }
      if (SZ(ends) == 2 && Point::LexCmp(ends[1], ends[0])) { return vector<Point>(); }
      return ends;
    } else {
      vector<Point> p = InterLineLine(l1, l2);
      if (PtBelongToSeg(l1, p[0]) && PtBelongToSeg(l2, p[0])) { return p; }
      return vector<Point>();
    }
  }

  static LD Angle(Point P, Point Q, Point R) { // angle PQR
    LD ang2 = (P - Q).Angle();
    LD ang1 = (R - Q).Angle();
    LD ans = ang1 - ang2;
    if (ans < kEps) {
      ans += 2 * kPi;
    }
    return ans;
  }

  // tested here: http://codeforces.com/contest/600/submission/14961583
  // DON'T change anything as this will lead to precision errors
  // don't know why, but this is the only version which works precisely even for very mean cases
  static LD DiskInterArea(Circle c1, Circle c2) { // tested here: http://opentrains.snarknews.info/~ejudge/team.cgi?contest_id=006254 problem I
    if (c1.r < c2.r) {
      swap(c1, c2);
    }
    LD d = c1.center.Dist(c2.center);
    if (c1.r + c2.r < d + kEps) {
      return 0;
    }
    if (c1.r - c2.r > d - kEps) {
      return kPi * Sq(c2.r);
    }
    LD alfa = acos((Sq(d) + Sq(c1.r) - Sq(c2.r)) / (2 * d * c1.r));
    LD beta = acos((Sq(d) + Sq(c2.r) - Sq(c1.r)) / (2 * d * c2.r));
    return alfa * Sq(c1.r) + beta * Sq(c2.r) - sin(2 * alfa) * Sq(c1.r) / 2 - sin(2 * beta) * Sq(c2.r) / 2;
  }

  static Line RadAxis(Circle c1, Circle c2) {
    LD d = c1.center.Dist(c2.center);
    LD a = (Sq(c1.r) - Sq(c2.r) + Sq(d)) / (2 * d);
    Point Q = c1.center + (c2.center - c1.center) * (a / d);
    Point R = Q + (c2.center - c1.center).Rotate(kPi / 2);
    return Line(Q, R);
  }
};

struct Polygon {
  vector<Point> pts;
  Polygon(vector<Point> pts_) : pts(pts_) {}
  Polygon() : Polygon(vector<Point>()) {}
  void Add(Point p) {
    pts.push_back(p);
  }
  // positive for counterclockwise
  double Area() {
    double area = 0;
    for (int i = 0; i < SZ(pts); i++) {
      area += pts[i].CrossProd(pts[(i + 1) % SZ(pts)]);
    }
    area /= 2;
    return area;
  }
  void OrientCounterclockwise() {
    if (Area() < 0) {
      reverse(pts.begin(), pts.end());
    }
  }
  int next(int a) {
    if (a + 1 < SZ(pts)) {
      return a + 1;
    }
    return 0;
  }
  pair<int, int> FurthestPair() { // tested here: http://codeforces.com/contest/333/submission/11058065
    MakeConvexHull();
    OrientCounterclockwise();
    int furth = 1;
    pair<int, int> best_pair = make_pair(0, 0);
    double best_dis = 0;
    for (int i = 0; i < SZ(pts); i++) {
      Point side = pts[next(i)] - pts[i];
      while (side.CrossProd(pts[furth] - pts[i]) < side.CrossProd(pts[next(furth)] - pts[i])) {
        furth = next(furth);
      }
      vector<int> vec{i, next(i)};
      for (auto ind : vec) {
        if (pts[ind].Dist(pts[furth]) > best_dis) {
          best_pair = make_pair(ind, furth);
          best_dis = pts[ind].Dist(pts[furth]);
        }
      }
      cerr<<"Furthest from: "<<pts[i]<<"-"<<pts[next(i)]<<" is "<<pts[furth]<<endl;
    }
    return best_pair;
  }
  // for square 34
  //            12 holds one_way_hull = {{1,3,4},{1,2,4}}
  // resulting polygon is counterclockwise {1, 2, 4, 3}
  vector<vector<Point>> MakeConvexHull() { // tested everywhere http://codeforces.com/contest/333/submission/11058065
    vector<vector<Point>> one_way_hull(2);
    sort(pts.begin(), pts.end(), Point::LexCmp);
    for (int dir = -1; dir <= 1; dir += 2) {
      int hull_num = (dir + 1) / 2;
      auto& H = one_way_hull[hull_num];
      one_way_hull[hull_num].push_back(pts[0]);
      if (SZ(pts) > 1) {
        H.push_back(pts[1]);
      }
      for (int i = 2; i < SZ(pts); i++) {
        while (SZ(H) >= 2 &&
            dir * (pts[i] - H[SZ(H) - 2]).CrossProd(H.back() - H[SZ(H) - 2]) > -kEps) {
          H.pop_back();
        }
        H.push_back(pts[i]);
      }
      if (SZ(H) > 1 && (H[0] - H.back()).IsZero()) { H.pop_back(); }
    }
    pts.clear();
    for (auto p : one_way_hull[1]) {
      pts.push_back(p);
    }
    for (int i = SZ(one_way_hull[0]) - 2; i >= 1; i--) {
      pts.push_back(one_way_hull[0][i]);
    }
    return one_way_hull;
  }

  // without sides
  vector<vector<bool>> InsideDiagonalsMatrix() { // tested here: http://codeforces.com/contest/438/submission/11063385
    int n = pts.size();
    vector<vector<bool>> res(n, vector<bool>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        Line diag(pts[i], pts[j]);
        if (i == j || abs(i - j) == 1 || abs(i - j) == n - 1) { continue; }
        res[i][j] = 1;
        for (int k = 0; k < n; k++) {
          int kk = next(k);
          Line side(pts[k], pts[kk]);
          if (k == i || k == j || kk == i || kk == j) { continue; }
          vector<Point> inter = Utils::InterSegs(diag, side);
          if (SZ(inter)) { res[i][j] = 0; }
        }
        int act = next(i);
        LD areas[2] = {0, 0};
        int passed_j = 0;
        while (act != i) {
          passed_j |= (act == j);
          areas[passed_j] += pts[i].CrossProd(pts[act], pts[next(act)]);
          act = next(act);
        }
        if (areas[0] * areas[1] < kEps) {
          res[i][j] = 0;
        }
      }
    }
    return res;
  }

  // P needs to be strictly outside polygon
  // polygon needs to be STRICTLY convex and counterclockwise oriented (as MakeConvexHull does)
  // returns {L, R} so that PL, PR are tangents and PL is on left
  vector<Point> Tangents(Point p) { // tested here: https://icpc.kattis.com/problems/spin (1169964)
    vector<Point> res;
    REP (tr, 2) {
      auto GrThan = [&](int fir, int sec) { // fir on sec's left
        return p.CrossProd(pts[sec], pts[fir]) > kEps;
      };
      bool up = false;
      int cr = 0;
      if (SZ(pts) >= 2) { cr = p.CrossProd(pts[0], pts[1]); }
      if (abs(cr) < kEps && SZ(pts) >= 3) { cr = p.CrossProd(pts[0], pts[2]); }
      up = (cr > 0);
      VI bd{1, SZ(pts) - 1};
      int faj = 0;
      while (bd[0] + 6 <= bd[1]) { // better don't replace with smaller constants
        VI h(2);
        REP (hh, 2) { h[hh] = (bd[0] + bd[1] + bd[hh]) / 3; }
        if (!GrThan(h[up ^ tr], 0) ^ tr) { bd[up ^ tr] = h[up ^ tr]; }
        else {
          int gr = GrThan(h[0], h[1]);
          bd[gr ^ tr] = h[gr ^ tr];
        }
      }
      FOR (i, bd[0], bd[1]) {
        if (GrThan(i, faj) ^ tr) {
          faj = i;
        }
      }
      res.PB(pts[faj]);
    }
    return res;
  }
};

struct ConvexPolHalves { // tested here: https://icpc.kattis.com/problems/spin (1169964)
  vector<vector<Point>> chains; // initialized by MakeConvexHull
  bool BelongTo(Point p) { // including borders
    if (SZ(chains[0]) == 1) {
      return (chains[0][0] - p).IsZero();
    }
    if (p.x  + kEps < chains[0][0].x || p.x - kEps > chains[0].back().x) { return false; }
    REP (tr, 2) {
      int kl = 0, kp = SZ(chains[tr]) - 2, faj = 0;
      while (kl <= kp) {
        int aktc = (kl + kp) / 2;
        if (chains[tr][aktc].x < p.x + kEps) {
          kl = aktc + 1;
          faj = aktc;
        } else {
          kp = aktc - 1;
        }
      }
      Point fir = chains[tr][faj], sec = chains[tr][faj + 1];
      if (abs(fir.x - sec.x) < kEps) {
        if (tr == 0) { if (sec.y + kEps < p.y) { return false; } }
        else { if (fir.y - kEps > p.y) { return false; } }
      } else {
        LD cr = fir.CrossProd(sec, p);
        if (abs(cr) < kEps) { return true; }
        if ((cr > 0) ^ tr) { return false; }
      }
    }
    return true;
  }
};

// CLIP START
bool InUpper(Point a) {
  if (abs(a.y) > kEps) {
    return a.y > 0;
  }
  return a.x > 0;
}

bool angle_cmp(const Point a, const Point b) {
  bool u = InUpper(a);
  bool v = InUpper(b);
  return u!=v ? u : a.CrossProd(b)>0;
}

/**
  * @brief a+(b-a)*f \in c+lin(d-c)
  * @returns f
  */
LD cross(Point a, Point b, Point c, Point d) {
  return (d - c).CrossProd(a - c) / (d - c).CrossProd(a - b);
}

struct ClipLine { // valid side is on left
  ClipLine(Point A, Point B) : al(A), bl(B), a(A), b(B) {};
  Point al,bl; // original line points
  mutable Point a,b; // actual intersection points
  Point dir() const { return bl - al; }
  bool operator<(const ClipLine& l) const { return angle_cmp(dir(),l.dir()); }
  Point cross(const ClipLine& l) {
    return al + (bl - al) * ::cross(al, bl, l.al, l.bl);
  }
  bool left(Point p) {
    return (bl - al).CrossProd(p - al) > 0;
  }
};

struct Clip {
  Clip(LD r) : area(4*r*r) {
    Point a{-r,-r}, b{r,-r}, c{r,r}, d{-r,r};
    lines = {ClipLine(a,b), ClipLine(b,c), ClipLine(c,d), ClipLine(d,a)};
  }

  void insert(Line l) { insert(ClipLine(l[0], l[1])); }

  void insert(ClipLine l) {
    assert(abs(l.dir().SqNorm()) > kEps);
    find(l);
    while (size() && !l.left(it->a) && !l.left(it->b)) { erase(); }
    if (size()) {
      while (prev(), size() && !l.left(it->a) && !l.left(it->b)) { erase(); }
    }
    if (size() && (!l.left(it->a) || !l.left(it->b))) {
      l.a = l.cross(*it);
      area -= l.a.CrossProd(it->b)*.5; it->b = l.a; next();
      l.b = l.cross(*it);
      if ((l.a-l.b).SqNorm() < kEps) {
        l.b = l.a;
      }
      area -= it->a.CrossProd(l.b) * .5;
      it->a = l.b;
      if (!(l.a - l.b).IsZero()) {
        area += l.a.CrossProd(l.b)*.5;
        lines.insert(l);
      }
    }
    //assert(l.dir().SqNorm()>1e-13);
  }

  void find(const ClipLine &l) {
    it = lines.lower_bound(l);
    if (it == lines.end()) { it = lines.begin(); }
  }

  void recalculate() {
    area = 0; for (const ClipLine &l : lines) area += l.a.CrossProd(l.b);
    area *= .5;
  }

  int size() { return lines.size(); }
  void next() { if(++it==lines.end()) it = lines.begin(); }
  void prev() { if(it==lines.begin()) it = lines.end(); --it; }
  void erase() {
      assert(it!=lines.end());
      area -= it->a.CrossProd(it->b)*.5;
      it = lines.erase(it);
      if(it==lines.end()) it = lines.begin();
  }
  typename set<ClipLine>::iterator it;
  set<ClipLine> lines;
  LD area;
};
// CLIP ENDS

// CENTERS BEGIN
Point Bary(Point A, Point B, Point C, LD a, LD b, LD c) {
    return (A * a + B * b + C * c) / (a + b + c);
}

Point Centroid(Point A, Point B, Point C) {
    return Bary(A, B, C, 1, 1, 1);
}

Point Circumcenter(Point A, Point B, Point C) {
    LD a = (B - C).SqNorm(), b = (C - A).SqNorm(), c = (A - B).SqNorm();
    return Bary(A, B, C, a * (b + c - a), b * (c + a - b), c * (a + b - c));
}

Point Incenter(Point A, Point B, Point C) {
    return Bary(A, B, C, (B - C).Norm(), (A - C).Norm(), (A - B).Norm());
}

Point Orthocenter(Point A, Point B, Point C) {
    LD a = (B - C).SqNorm(), b = (C - A).SqNorm(), c = (A - B).SqNorm();
    return Bary(A, B, C, (a+b-c)*(c+a-b), (b+c-a)*(a+b-c), (c+a-b)*(b+c-a));
}

Point Excenter(Point A, Point B, Point C) { // opposite to A
    LD a = (B - C).Norm(), b = (A - C).Norm(), c = (A - B).Norm();
    return Bary(A, B, C, -a, b, c);
}

struct Point3 {
  LD x, y, z;
  Point3 operator+(Point3 a) { Point3 p{x + a.x, y + a.y, z + a.z}; return p; }
  Point3 operator-(Point3 a) { Point3 p{x - a.x, y - a.y, z - a.z}; return p; }
  Point3 operator*(LD a) { Point3 p{x * a, y * a, z * a}; return p; }
  Point3 operator/(LD a) { assert(a > kEps); Point3 p{x / a, y / a, z / a}; return p; }
  Point3& operator+=(Point3 a) { x += a.x; y += a.y; z += a.z; return *this; }
  Point3& operator-=(Point3 a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
  Point3& operator*=(LD a) { x *= a; y *= a; z *= a; return *this;}
  Point3& operator/=(LD a) { assert(a > kEps); x /= a; y /= a; z /= a; return *this; }

  LD& operator[](int a) {
    if (a == 0) { return x; }
    if (a == 1) { return y; }
    if (a == 2) { return z; }
    assert(false);
  }
  bool IsZero() {
    return abs(x) < kEps && abs(y) < kEps && abs(z) < kEps;
  }
  bool operator==(Point3 a) {
    return (*this - a).IsZero();
  }
  LD DotProd(Point3 a) {
    return x * a.x + y * a.y + z * a.z;
  }
  LD Norm() {
    return sqrt(x * x + y * y + z * z);
  }
  LD SqNorm() {
    return x * x + y * y + z * z;
  }
  void NormalizeSelf() {
    *this /= Norm();
  }
  Point3 Normalize() {
    Point3 res(*this);
    res.NormalizeSelf();
    return res;
  }
  LD Dis(Point3 a) {
    return (*this - a).Norm();
  }
  pair<LD, LD> SphericalAngles() {
    return {atan2(z, sqrt(x * x + y * y)), atan2(y, x)};
  }
  LD Area(Point3 p) {
    return Norm() * p.Norm() * sin(Angle(p)) / 2;
  }
  LD Angle(Point3 p) {
    LD a = Norm();
    LD b = p.Norm();
    LD c = Dis(p);
    return acos((a * a + b * b - c * c) / (2 * a * b));
  }
  static LD Angle(Point3 p, Point3 q) {
    return p.Angle(q);
  }
  Point3 CrossProd(Point3 p) {
    Point3 q(*this);
    return {q[1] * p[2] - q[2] * p[1], q[2] * p[0] - q[0] * p[2], q[0] * p[1] - q[1] * p[0]};
  }
  static bool LexCmp(Point3& a, const Point3& b) {
    if (abs(a.x - b.x) > kEps) { return a.x < b.x; }
    if (abs(a.y - b.y) > kEps) { return a.y < b.y; }
    return a.z < b.z;
  }

  friend ostream& operator<<(ostream& out, Point3 m);
};

bool IsZero(Point3 A) {
  return abs(A.x) < kEps && abs(A.y) < kEps && abs(A.z) < kEps;
}

ostream& operator<<(ostream& out, Point3 p) {
  out << "(" << p.x << ", " << p.y << ", " << p.z << ")";
  return out;
}

struct Line3 {
  Point3 p[2];
  Point3& operator[](int a) { return p[a]; }
  friend ostream& operator<<(ostream& out, Line3 m);
};

ostream& operator<<(ostream& out, Line3 l) {
  out << l[0] << " - " << l[1];
  return out;
}

struct Plane {
  Point3 p[3];
  Point3& operator[](int a) { return p[a]; }
  Point3 GetNormal() {
    Point3 cross = (p[1] - p[0]).CrossProd(p[2] - p[0]);
    return cross.Normalize();
  }
  void GetPlaneEq(LD& A, LD& B, LD& C, LD& D) {
    Point3 normal = GetNormal();
    A = normal[0];
    B = normal[1];
    C = normal[2];

    D = normal.DotProd(p[0]);
    assert(abs(D - normal.DotProd(p[1])) < kEps);
    assert(abs(D - normal.DotProd(p[2])) < kEps);
  }
  vector<Point3> GetOrtonormalBase() {
    Point3 normal = GetNormal();
    Point3 cand = {-normal.y, normal.x, 0};
    if (abs(cand.x) < kEps && abs(cand.y) < kEps) {
      cand = {0, -normal.z, normal.y};
    }
    cand.NormalizeSelf();
    Point3 third = Plane{Point3{0, 0, 0}, normal, cand}.GetNormal();
    assert(abs(normal.DotProd(cand)) < kEps && abs(normal.DotProd(third)) < kEps && abs(cand.DotProd(third)) < kEps);
    return {normal, cand, third};
  }
};


struct Circle3 {
  Plane pl;
  Point3 cent;
  LD r;
  friend ostream& operator<<(ostream& out, Circle3 m);
};

ostream& operator<<(ostream& out, Circle3 c) {
  out << "pl: (" << c.pl[0] << ", " << c.pl[1] << ", " << c.pl[2] << "), cent: " << c.cent << " r: " << c.r << "\n";
  return out;
}

struct Sphere {
  Point3 cent;
  LD r;
};

struct Utils3 {
  static bool Lines3Equal(Line3 p, Line3 l) {
    return Utils3::PtBelongToLine3(p[0], l) && Utils3::PtBelongToLine3(p[1], l);
  }
  //angle PQR
  static LD Angle(Point3 P, Point3 Q, Point3 R) {
    return (P - Q).Angle(R - Q);
  }
  static Point3 ProjPtToLine3(Point3 p, Line3 l) { // ok
    Point3 diff = l[1] - l[0];
    diff.NormalizeSelf();
    return l[0] + diff * (p - l[0]).DotProd(diff);
  }
  static LD DisPtLine3(Point3 p, Line3 l) { // ok
//    LD area = Area(p, l[0], l[1]);
//     LD dis1 = 2 * area / l[0].Dis(l[1]);
    LD dis2 = p.Dis(ProjPtToLine3(p, l));
//     assert(abs(dis1 - dis2) < kEps);
    return dis2;
  }
  static LD DisPtPlane(Point3 p, Plane pl) {
    Point3 normal = pl.GetNormal();
    return abs(normal.DotProd(p - pl[0]));
  }
  static Point3 ProjPtToPlane(Point3 p, Plane pl) {
    Point3 normal = pl.GetNormal();
    return p - normal * normal.DotProd(p - pl[0]);
  }
  static bool PtBelongToPlane(Point3 p, Plane pl) {
    return DisPtPlane(p, pl) < kEps;
  }
  static Point PlanePtTo2D(Plane pl, Point3 p) { // ok
    assert(PtBelongToPlane(p, pl));
    vector<Point3> base = pl.GetOrtonormalBase();
    Point3 control{0, 0, 0};
    REP (tr, 3) {
      control += base[tr] * p.DotProd(base[tr]);
    }
    assert(PtBelongToPlane(pl[0] + base[1], pl));
    assert(PtBelongToPlane(pl[0] + base[2], pl));
    assert((p - control).IsZero());
    return {p.DotProd(base[1]), p.DotProd(base[2])};
  }
  static Line PlaneLineTo2D(Plane pl, Line3 l) {
    return {PlanePtTo2D(pl, l[0]), PlanePtTo2D(pl, l[1])};
  }
  static Point3 PlanePtTo3D(Plane pl, Point p) { // ok
    vector<Point3> base = pl.GetOrtonormalBase();
    return base[0] * base[0].DotProd(pl[0]) + base[1] * p.x + base[2] * p.y;
  }
  static Line3 PlaneLineTo3D(Plane pl, Line l) {
    return {PlanePtTo3D(pl, l[0]), PlanePtTo3D(pl, l[1])};
  }


  static Line3 ProjLineToPlane(Line3 l, Plane pl) { // ok
    return {ProjPtToPlane(l[0], pl), ProjPtToPlane(l[1], pl)};
  }
  static LD DisLineLine(Line3 l, Line3 k) { // ok
    Plane together {l[0], l[1], l[0] + k[1] - k[0]}; // parallel FIXME
    Line3 proj = ProjLineToPlane(k, together);
    Point3 inter = (Utils3::InterLineLine(l, proj))[0];
    Point3 on_k_inter = k[0] + inter - proj[0];
    return inter.Dis(on_k_inter);
  }
//
  static bool PtBelongToLine3(Point3 p, Line3 l) {
    return DisPtLine3(p, l) < kEps;
  }
  static bool Line3BelongToPlane(Line3 l, Plane pl) {
    return PtBelongToPlane(l[0], pl) && PtBelongToPlane(l[1], pl);
  }
  static LD Det(Point3 a, Point3 b, Point3 d) { // ok
    Point3 pts[3] = {a, b, d};
    LD res = 0;
    for (int sign : {-1, 1}) {
      REP (st_col, 3) {
        int c = st_col;
        LD prod = 1;
        REP (r, 3) {
          prod *= pts[r][c];
          c = (c + sign + 3) % 3;
        }
        res += sign * prod;
      }
    }
    return res;
  }
  static LD Det(Point3 A, Point3 B, Point3 C, Point3 D) {
    B = B - A;
    C = C - A;
    D = D - A;
    return Det(B, C, D);
  }
  static LD Area(Point3 p, Point3 q, Point3 r) { // ok
    q -= p;
    r -= p;
    return q.Area(r);
  }
  static Point3 PtFromSphericalAng(LD alpha, LD beta) { // ok
    return {cos(alpha) * cos(beta), cos(alpha) * sin(beta), sin(alpha)};
  }
  static vector<Point3> InterLineLine(Line3 k, Line3 l) {
    if (Lines3Equal(k, l)) { return {k[0], k[1]}; }
    if (PtBelongToLine3(l[0], k)) { return {l[0]}; }
    Plane pl{l[0], k[0], k[1]};
    if (!PtBelongToPlane(l[1], pl)) { return {}; }
    Line k2 = PlaneLineTo2D(pl, k);
    Line l2 = PlaneLineTo2D(pl, l);
    vector<Point> inter = Utils::InterLineLine(k2, l2);
    vector<Point3> res;
    for (auto P : inter) { res.PB(PlanePtTo3D(pl, P)); }
    return res;
  }

  static Plane ParallelPlane(Plane pl, Point3 A) { // plane parallel to pl going through A
    Point3 diff = A - ProjPtToPlane(A, pl);
    return {pl[0] + diff, pl[1] + diff, pl[2] + diff};
  }

  // image of B in rotation wrt line passing through origin s.t. A1->A2
  // implemented in more general case with similarity instead of rotation
  static Point3 RotateAccordingly(Point3 A1, Point3 A2, Point3 B1) { // ok
    Plane pl{A1, A2, {0, 0, 0}};
    Point A12 = PlanePtTo2D(pl, A1);
    Point A22 = PlanePtTo2D(pl, A2);
    complex<LD> rat = complex<LD>(A22.x, A22.y) / complex<LD>(A12.x, A12.y);
    Plane plb = ParallelPlane(pl, B1);
    Point B2 = PlanePtTo2D(plb, B1);
    complex<LD> Brot = rat * complex<LD>(B2.x, B2.y);
    return PlanePtTo3D(plb, {Brot.real(), Brot.imag()});
  }

  static vector<Circle3> InterSpherePlane(Sphere s, Plane pl) { // ok
    Point3 proj = ProjPtToPlane(s.cent, pl);
    LD dis = s.cent.Dis(proj);
    if (dis > s.r + kEps) {
      //return {{{}, {}, {}}, {}, -1};
      return {};
    }
    if (dis > s.r - kEps) {
      //return {{{}, {}, {}}, proj, 0};
      return {{pl, proj, 0}}; // is it best choice?
    }
    return {{pl, proj, sqrt(s.r * s.r - dis * dis)}};
  }

  static bool PtBelongToSphere(Sphere s, Point3 p) {
    return abs(s.r - s.cent.Dis(p)) < kEps;
  }

  static LD DisOnSphere(Sphere sph, Point3 A, Point3 B) {
    assert(PtBelongToSphere(sph, A));
    assert(PtBelongToSphere(sph, B));
    LD ang = Angle(A, sph.cent, B);
    return ang * sph.r;
  }



};

vector<Line3> InterPlanePlane(Plane A, Plane B) {
  vector<Point3> pts;
  REP (i, 3) {
    pts.PB(A[i]);
  }
  REP (i, 2) {
    //Line3 l{A[i], A[i + 1]};
    Point3 oth = A[(i + 2) % 3];
    Point3 fourth = A[i + 1] + oth - A[i];
    vector<Line3> ls{{A[i], A[i + 1]}, {oth, fourth}};
    vector<Point3> lols;
    REP (j, 2) {
      Line3& l = ls[j];
      LD v1 = Utils3::Det(B[0], B[1], B[2], l[0]);
      LD v2 = Utils3::Det(B[0], B[1], B[2], l[1]);
      if (abs(v1 - v2) < kEps) { break; }
      lols.PB(l[0] + (l[1] - l[0]) * (v1 / (v1 - v2)));
    }
    if (SZ(lols) == 2) {
      assert(Utils3::PtBelongToPlane(lols[0], B));
      assert(Utils3::PtBelongToPlane(lols[1], B));
      assert(Utils3::PtBelongToPlane(lols[0], A));
      assert(Utils3::PtBelongToPlane(lols[1], A));
      assert(!IsZero(lols[0] - lols[1]));
      return vector<Line3>{{lols[0], lols[1]}};
    }
  }
  return {};
}


struct PointS { // just for conversion purposes, probably toEucl suffices
  LD lat, lon;
  Point3 toEucl() {
    return Point3{cos(lat) * cos(lon), cos(lat) * sin(lon), sin(lat)};
  }
  PointS(Point3 p) {
    p.NormalizeSelf();
    lat = asin(p.z);
    lon = acos(p.y / cos(lat));
  }
};

LD DistS(Point3 a, Point3 b) {
  return atan2l(b.CrossProd(a).Norm(), a.DotProd(b));
}

struct CircleS {
  Point3 o; // center of circle on sphere
  LD r; // arc len
  LD area() const { return 2 * kPi * (1 - cos(r)); }
};

CircleS From3(Point3 a, Point3 b, Point3 c) { // any three different points
  int tmp = 1;
  if ((a - b).Norm() > (c - b).Norm()) { swap(a, c); tmp = -tmp; }
  if ((b - c).Norm() > (a - c).Norm()) { swap(a, b); tmp = -tmp; }
  Point3 v = (c - b).CrossProd(b - a); v = v * (tmp / v.Norm());
  return CircleS{v, DistS(a,v)};
}

CircleS From2(Point3 a, Point3 b) { // neither the same nor the opposite
  Point3 mid = (a + b) / 2;
  mid = mid / mid.Norm();
  return From3(a, mid, b);
}

LD Angle(Point3 A, Point3 B, Point3 C) { //angle at A, no two points opposite
  LD a = B.DotProd(C);
  LD b = C.DotProd(A);
  LD c = A.DotProd(A);
  return acos((b - a * c) / sqrt(max((LD)0, (1 - Sq(a)) * (1 - Sq(c)))));
}

LD TriangleArea(Point3 A, Point3 B, Point3 C) { // no two poins opposite
  LD a = Angle(C, A, B);
  LD b = Angle(A, B, C);
  LD c = Angle(B, C, A);
  return a + b + c - kPi;
}

vector<Point3> IntersectionS(CircleS c1, CircleS c2) {
  Point3 n = c2.o.CrossProd(c1.o), w = c2.o * cos(c1.r) - c1.o * cos(c2.r);
  LD d = n.SqNorm();
  if (d < kEps) { // parallel circles (can fully overlap)
    return {};
  }
  LD a = w.SqNorm() / d;
  vector<Point3> res;
  if (a >= 1 + kEps) { return res; }
  Point3 u = n.CrossProd(w) / d;
  if (a > 1 - kEps) {
    res.PB(u);
    return res;
  }
  LD h = sqrt((1 - a) / d);
  res.PB(u + n * h);
  res.PB(u - n * h);
  return res;
}
