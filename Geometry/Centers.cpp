struct Centers {
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
        return Bary(A, B, C, (a + b - c) * (c + a - b), (b + c - a) * (a + b - c), (c + a - b) * (b + c - a));
    }

    Point Excenter(Point A, Point B, Point C) { // opposite to A
        LD a = (B - C).Norm(), b = (A - C).Norm(), c = (A - B).Norm();
        return Bary(A, B, C, -a, b, c);
    }
};
Centers CR;
