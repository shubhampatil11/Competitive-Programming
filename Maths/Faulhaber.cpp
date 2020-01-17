namespace faulhaber {
const int MAX = 1010;
int str[MAX][MAX];
int inverse[MAX];

//Stirling Numbers of Second Kind
void generate() {
    str[0][0] = 1;
    for(int i = 1; i < MAX; i++) {
        str[i][0] = 0;
        for(int j = 1; j <= i; j++) {
            str[i][j] = 0;
            add(str[i][j], mul(str[i - 1][j], j));
            add(str[i][j], str[i - 1][j - 1]);
        }
    }
    inverse[0] = 0;
    for(int i = 1; i < MAX; i++) {
        inverse[i] = inv(i);
    }
}

int get(long long n, int k) {
    n = (int)(n % (long long) mod);
    if(!k) {
        return n;
    }
    int ans = 0, p = 1;
    for(int i = 0; i <= k; i++) {
        p = mul(p, n + 1 - i);
        add(ans, mul(mul(str[k][i], p), inverse[i + 1]));
    }
    return ans;
}
}
