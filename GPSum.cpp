//{x^n, 1 + x + x^2 + .... + x^(n - 1)}
pair < int, int > GPSum(int x, int n) {
    if(x == 1) return {1, n};
    if(n == 1) return {x, 1};
    int r = 0, t = 1, s = 1;
    while(n) {
        if(n & 1) {
            r = (r + 1LL * t * s) % mod; t = (1LL * t * x) % mod;
        }
        n >>= 1;
        s = (1LL * s * (1 + x)) % mod;
        x = (1LL * x * x) % mod;
    }
    return {t, r};
}
