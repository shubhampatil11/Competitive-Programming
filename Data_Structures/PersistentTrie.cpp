int T[2][N * M] = {}, cnt = 0;

int add(int idx, int prev, int k) {
    cnt++;
    int root = cnt;
    if(idx < 0) {
        return root;
    }
    int r = (1 & (k >> idx));
    T[r ^ 1][root] = T[r ^ 1][prev];
    T[r][root] = add(idx - 1, T[r][prev], k);
    return root;
}

map<int,int> ROOT;

int get_min(int k, int u) {
    int bit = 0;
    int root = ROOT[u];
    for(int i = M - 1; i >= 0; i--) {
        int r = ((k >> i) & 1);
        if(T[r][root]) {
            bit += (r << i);
            root = T[r][root];
        }
        else {
            bit += ((r ^ 1) << i);
            root = T[r ^ 1][root];
        }
    }
    return bit;
}

int get_max(int k, int u) {
    int bit = 0;
    int root = ROOT[u];
    for(int i = M - 1; i >= 0; i--) {
        int r = ((k >> i) & 1);
        if(T[r ^ 1][root]) {
            bit += ((r ^ 1) << i);
            root = T[r ^ 1][root];
        }
        else {
            bit += (r << i);
            root = T[r][root];
        }
    }
    return bit;
}
