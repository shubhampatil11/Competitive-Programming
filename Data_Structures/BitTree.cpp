struct BIT {
    vector<int> T;
    int n;
    BIT(int _n = N) {
        T.resize(_n + 1, 0);
        n = _n;
    }
    int read(int idx){
        int sum = 0;
        while (idx > 0){
            sum += T[idx];
            idx -= (idx & -idx);
        }
        return sum;
    }
    void update(int idx, int val){
        while (idx <= n){
            T[idx] += val;
            idx += (idx & -idx);
        }
    }
};
