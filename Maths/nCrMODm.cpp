template <typename T>
T mod_inv(T a, T md) {
  T b = md, u = 1, v = 0;
  while (b != 0) {
    T t = a / b;
    a -= t * b; swap(a, b);
    u -= t * v; swap(u, v);
  }
  u %= md;
  if (u < 0) u += md;
  return u;
}

inline long long power(long long a, long long b, long long md) {
  long long res = 1;
  while (b > 0) {
    if (b & 1) {
      res = (res * a) % md;
    }
    a = (a * a) % md;
    b >>= 1;
  }
  return res;
}

// initialize:O(md), query:O(log n)
pair<long long, long long> factorial(long long n, long long p, long long md) {
    static unordered_map<long long, vector<long long>> sum;
    if (!sum.count(md)) {
        auto& s = sum[md];
        s.assign(md, 1);
        for (int i = 1; i < md; i++)
            s[i] = (long long)(i % p) ? ((long long)s[i - 1] * i) % md : s[i - 1];
    }
    const auto& s = sum[md];
    long long cnt = 0, res = 1;
    while (n > 1) {
        (res *= power(s[md - 1], n / md, md)) %= md;
        (res *= s[n % md]) %= md;
        n /= p;
        cnt += n;
    }
    return make_pair(cnt, res);
}

long long nCr(long long n, long long r, long long p, int k) {// nCr mod p^k
    if ((long long)r < 0 || (long long)n < r) return 0;
    long long md = 1;
    for (int i = 0; i < k; i++) md *= p;
    auto a = factorial(n, p, md);
    auto b = factorial(r, p, md);
    auto c = factorial(n - r, p, md);
    long long res = a.second;
    (res *= mod_inv(b.second, md)) %= md;
    (res *= mod_inv(c.second, md)) %= md;
    int cnt = min(a.first - b.first - c.first, (long long)k);
    for (int i = 0; i < cnt; i++) (res *= p) %= md;
    return res;
}

typedef long long TP;
TP exgcd(TP a, TP b, TP &xcoef, TP &ycoef) {
    if(b == 0) {
        xcoef = 1;
        ycoef = 0;
        return a;
    }
    TP temp, gc = exgcd(b, a % b, xcoef, ycoef);
    temp = ycoef;
    ycoef = xcoef - (a / b) * ycoef;
    xcoef = temp;
    return gc;
}
bool crt(vector<TP> &a, vector<TP> &b, vector<TP> &m, TP &x, TP &M) {
    long long n = a.size();
    x = 0;
    M = 1;
    for(long long i = 0; i < n; i++) {
        long long a_ = a[i] * M, b_ = b[i] - a[i] * x, m_ = m[i];
        long long y, t, g = exgcd(a_, m_, y, t);
        if (b_ % g)return false;
        b_ /= g;
        m_ /= g;
        x += M * (y * b_ % m_);
        M *= m_;
    }
    x = (x + M) % M;
    return true;
}

long long nCr(long long n, long long r, long long mod) {
    vector<pair<long long,long long>> facts;
    vector<long long> mods;
    for(long long i = 2; i * i <= mod; i++) {
        if(mod % i == 0) {
            int cnt = 0;
            long long temp = 1;
            while(mod % i == 0) {
                mod /= i;
                cnt++;
                temp *= i;
            }
            mods.emplace_back(temp);
            facts.emplace_back(make_pair(i, cnt));
        }
    }
    if(mod > 1) {
        facts.emplace_back(mod, 1);
        mods.emplace_back(mod);
    }
    vector<long long> a, b;
    for(auto v : facts) {
        a.emplace_back(1);
        b.emplace_back(nCr(n, r, v.first, v.second));
    }
    long long ans;
    crt(a, b, mods, ans, mod);
    return ans;
}
