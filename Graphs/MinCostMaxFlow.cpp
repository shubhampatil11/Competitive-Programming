const int MAXN = 10005;
const int MAXM = 60000;

struct Edge {
  int v, c, w;
  Edge() {}
  Edge(int to, int cap, int cost) : v(to), c(cap), w(cost) {}
};

struct MinCostMaxFlow {
  int n, m, source, sink;
  vector<int> e[MAXN];
  Edge edge[MAXM * 2];

  void init(int n, int source, int sink) {
    this->n = n;
    this->m = 0;
    this->source = source;
    this->sink = sink;
    for (int i = 0; i < n; ++i) {
      e[i].clear();
    }
  }

  void add(int a, int b, int c, int w) {
    edge[m] = Edge(b, c, w);
    e[a].push_back(m++);
    edge[m] = Edge(a, 0, -w);
    e[b].push_back(m++);
  }

  bool mark[MAXN];
  int maxc[MAXN];
  int minw[MAXN];
  int pre[MAXN];
  int d[MAXN];

  bool _spfa() {
    queue<int> q;

    fill(mark, mark + n, false);
    fill(maxc, maxc + n, 0);
    fill(minw, minw + n, numeric_limits<int>::max());
    fill(pre, pre + n, -1);
    fill(d, d + n, 0);
    mark[source] = true;
    maxc[source] = numeric_limits<int>::max();
    minw[source] = 0;
    q.push(source);
    while (!q.empty()) {
      int cur = q.front();
      mark[cur] = false;
      q.pop();
      for (size_t i = 0; i < e[cur].size(); ++i) {
        int id = e[cur][i];
        int v = edge[id].v;
        int c = min(maxc[cur], edge[id].c);
        if (c == 0) {
          continue;
        }
        int w = minw[cur] + edge[id].w;
        if (minw[v] > w || (minw[v] == w && maxc[v] < c)) {
          maxc[v] = c;
          minw[v] = w;
          pre[v] = id;
          d[v] = d[cur] + 1;
          if (d[v] >= n) {
            return false;
          }
          if (!mark[v]) {
            mark[v] = true;
            q.push(v);
          }
        }
      }
    }

    return true;
  }

  pair<int, int> GetMinCostMaxFlow() {
    int sumc = 0, sumw = 0;

    while (true){
        _spfa();
        if(maxc[sink] == 0) {
        break;
      } else if (minw[sink] >= 0) {
        break;
      } else {
        int c = maxc[sink];
        sumc += c;
        sumw += c * minw[sink];
        int cur = sink;
        while (cur != source) {
          int id = pre[cur];
          edge[id].c -= c;
          edge[id ^ 1].c += c;
          cur = edge[id ^ 1].v;
        }
      }
    }
    return make_pair(sumc, sumw);
  }
};
