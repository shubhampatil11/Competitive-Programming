struct points {
    int x, y;
    points(int xx = 0, int yy = 0) : x(xx), y(yy) {}
}P[N];

int getDistance(int p1, int p2)
{
    return (P[p1].x - P[p2].x) * (P[p1].x - P[p2].x) + (P[p1].y - P[p2].y) * (P[p1].y - P[p2].y);
}

int closestPoints(int l, int r)
{
    int d = inf;
    if(r - l <= 3)
    {
        for(int i = l; i <= r; i++) for(int j = i + 1; j <= r; j++)
            d = min(d, getDistance(i, j));
        return d;
    }
    int mid = (l + r) >> 1;
    d = min(closestPoints(l, mid), closestPoints(mid + 1, r));
    struct points temp[r - l + 1];
    merge(P + l, P + mid + 1, P + mid + 1, P + r + 1, temp, [](points A, points B) -> bool {return A.y < B.y;} );
    for(int i = l; i <= r; i++) P[i] = temp[i - l];
    for(int i = l; i <= r; i++) for(int j = i + 1; j <= min(i + 6, r); j++)
        d = min(d, getDistance(i, j));
    return d;
}
