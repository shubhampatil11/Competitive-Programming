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

    void operator = (const Point& a) {
        x = a.x;
        y = a.y;
    }
    Point operator + (const Point& a) const {
        Point p(x + a.x, y + a.y);
        return p;
    }
    Point operator - (const Point& a) const {
        Point p(x - a.x, y - a.y);
        return p;
    }
    Point operator * (LD a) const {
        Point p(x * a, y * a);
        return p;
    }
    Point operator / (LD a) const {
        assert(abs(a) > kEps);
        Point p(x / a, y / a);
        return p;
    }
    Point& operator += (const Point& a) {
        x += a.x;
        y += a.y;
        return *this;
    }
    Point& operator -= (const Point& a) {
        x -= a.x;
        y -= a.y;
        return *this;
    }
    Point& operator *= (LD a) {
        x *= a;
        y *= a;
        return *this;
    }
    Point& operator /= (LD a) {
        assert(abs(a) > kEps);
        x /= a;
        y /= a;
        return *this;
    }

    bool IsZero() const {
        return abs(x) < kEps && abs(y) < kEps;
    }
    bool operator == (const Point& a) const {
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
    Point perp() {
        return Point(-y, x);
    }
    //Counter-Clockwise rotation
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
        if (abs(a.x - b.x) > kEps)
            return a.x < b.x;
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

