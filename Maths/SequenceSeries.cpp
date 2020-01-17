namespace sequence {

//{x^n, 1 + x + x^2 + .... + x^(n - 1)}
pair <int,int> GPSum(int x, int n) {
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

// {x^n, 1 + x + x^2 + ... + x^(n-1), x + 2x^2 + 3x^3 + ... + (n-1)x^(n-1)}
pair<pair<int,int>,int> AGP(int x, int n) {
    if (x == 1) {
        return {{1, n}, (int)((ll)n * (n - 1) / 2 % mod)};
    }
    if (n == 1) {
        return {{x, 1}, 0};
    }
    int r0 = 0, r1 = 0, t = 1, a = 0;
    int s0 = 1, s1 = 0, b = 1;
    while (n) {
        if (n & 1) {
            r0 = (r0 + (ll)t * s0) % mod;
            r1 = (r1 + (ll)t * s1 + (ll)t * s0 % mod * a) % mod;
            t = ((ll)t * x) % mod;
            a = b;
        }
        n >>= 1;
        s1 = ((ll)s1 * (x + 1) + (ll)s0 * x % mod * b) % mod;
        s0 = (ll)s0 * (x + 1) % mod;
        x = (ll)x * x % mod;
        b <<= 1;
    }
    return {{t, r0}, r1};
}

}
