struct Bit2D {
    vector<vector<int>> tree;
    int n, m;

    Bit2D(int _n = N, int _m = N) : n(_n), m(_m) {
        tree.resize(n);
        for(int i = 0; i < n; i++) {
            tree[i].resize(m, 0);
        }
    }

    void update(int x, int y, int val) {
        while(x <= n) {
            int _y = y;
            while(y <= m) {
                tree[x][y] += val;
                y += (y & -y);
            }
            x += (x & -x);
            y = _y;
        }
    }

    int read(int x, int y) {
        int ans = 0;
        while(x > 0) {
            int _y = y;
            while(y > 0) {
                ans += tree[x][y];
                y -= (y & -y);
            }
            y = _y;
            x -= (x & -x);
        }
        return ans;
    }
};
