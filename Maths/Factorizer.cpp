namespace factorizer {

vector<int> least = {0, 1};
vector<int> primes;
int precalculated = 1;

void RunLinearSieve(int n) {
  n = max(n, 1);
  least.assign(n + 1, 0);
  primes.clear();
  for (int i = 2; i <= n; i++) {
    if (least[i] == 0) {
      least[i] = i;
      primes.push_back(i);
    }
    for (int x : primes) {
      if (x > least[i] || i * x > n) {
        break;
      }
      least[i * x] = x;
    }
  }
  precalculated = n;
}

void RunSieve(int n) {
  RunLinearSieve(n);
}

template <typename T>
vector<pair<T, int>> Factorize(T x) {
  vector<pair<T, int>> ret;
  for (T i : primes) {
    T t = x / i;
    if (i > t) {
      break;
    }
    if (x == t * i) {
      int cnt = 0;
      while (x % i == 0) {
        x /= i;
        cnt++;
      }
      ret.emplace_back(i, cnt);
    }
  }
  if (x > 1) {
    ret.emplace_back(x, 1);
  }
  return ret;
}

}
