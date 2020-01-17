/*
Base : 153, 23, 59, 61
MOD  : 1000000007, 1000000009, 1000001, 504095869, 412698983
*/
struct SingleHash {
    vector<ll> suffix, p;
    ll base, mod;
    int n;

    SingleHash(const string &s, ll _base = 153, ll _mod = 1e9 + 7) {
        base = _base;
        mod = _mod;
        n = (int)s.size();
        suffix.resize(n + 1, 0LL);
        p.resize(n + 1, 0LL);
        for(int i = n - 1; i >= 0; i--) {
            suffix[i] = (1LL * s[i] + suffix[i + 1] * base) % mod;
        }
        p[0] = 1LL;
        for(int i = 1; i <= n; i++) {
            p[i] = (p[i - 1] * base) % mod;
        }
    }

    ll get(int l, int r) {
        assert(l >= 0 && l < n && r >= 0 && r < n);
        ll ans = (suffix[l] - suffix[r + 1] * p[r + 1 - l]) % mod;
        ans = (ans + mod) % mod;
        return ans;
    }
};
