int tree[N][N];
int n, m;

void updatey(int x, int y, int val) {
    while(y <= m) {
        tree[x][y] += val;
        y += (y & -y);
    }
}

void updatex(int x, int y, int val) {
    while(x <= n) {
        updatey(x, y, val);
        x += (x & -x);
    }
}

int ready(int x, int y) {
    int ans = 0;
    while(y > 0) {
        ans += tree[x][y];
        y -= (y & -y);
    }
    return ans;
}
int readx(int x, int y) {
    int ans = 0;
    while(x > 0) {
        ans += ready(x, y);
        x -= (x & -x);
    }
    return ans;
}
