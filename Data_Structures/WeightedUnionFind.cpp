class wunionfind {
  public:
    int *id, *sz;
    wunionfind(int n = N) {
        id = new int[n + 1];
        sz = new int[n + 1];
        for(int i = 0; i <= n; i++) {
            id[i] = i;
            sz[i] = 1;
        }
    }
    int root(int idx) {
        int x = idx;
        while(x != id[x]) {
            id[x] = id[id[x]];
            x = id[x];
        }
        return x;
    }
    bool uni(int a, int b) {
        int x = root(a), y = root(b);
        if(sz[x] < sz[y]) {
            swap(x, y);
        }
        if (x != y) {
            id[y] = x;
            sz[x] += sz[y];
            sz[y] = 0;
            return false;
        }
        return true;
    }
    bool check(int a, int b) {
        return (root(a) == root(b));
    }
};
