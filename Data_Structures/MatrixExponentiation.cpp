inline void add(int &a, int b) {
  a += b;
  if(a >= mod) {
    a -= mod;
  }
  if(a < 0) {
    a += mod;
  }
}

inline int mul(int a, int b) {
  return (long long) a * b % mod;
}

struct Matrix {
  vector<vector<int>> a;
  int n, m;

  Matrix(int diag = 1, int _n = 2, int _m = 2) {
    n = _n; m = _m;
    a.resize(n);
    for(int i = 0; i < n; i++) {
      a[i].resize(m, 0);
    }
    for(int i = 0; i < min(n, m); i++) {
      a[i][i] = diag;
    }
  }
  Matrix(vector<vector<int>> &b) {
    n = (int)b.size(); m = (int)b[0].size();
    a = b;
  }
};

inline Matrix mul(Matrix &a, Matrix b) {
  assert(a.m == b.n);
  Matrix c(0, a.n, b.m);
  for(int i = 0; i < a.n; i++) {
    for(int j = 0; j < a.m; j++) {
      for(int k = 0; k < b.m; k++) {
        add(c.a[i][k], mul(a.a[i][j], b.a[j][k]));
      }
    }
  }
  return c;
}

inline Matrix power(Matrix a, long long b) {
  Matrix res(1);
  while(b > 0) {
    if(b & 1) {
      res = mul(res, a);
    }
    a = mul(a, a);
    b >>= 1;
  }
  return res;
}
