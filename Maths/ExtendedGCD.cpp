void solve(long long a, long long b, long long &x, long long &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return;
  }
  long long xx, yy;
  solve(b, a % b, xx, yy);
  // b * xx + (a - (a / b) * b) * yy == 1
  // a * yy + b * (xx - yy * (a / b)) == 1
  x = yy;
  y = xx - yy * (a / b);
}
