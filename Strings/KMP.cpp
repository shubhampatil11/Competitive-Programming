vector<int> build_failure_function(string &pattern) {
    int m = pattern.length();
    vector<int> F(m + 1, 0);
    F[0] = F[1] = 0;
    for(int i = 2; i <= m; i++) {
        int j = F[i - 1];
        while(1) {
            if(pattern[j] == pattern[i - 1]) {
                F[i] = j + 1;
                break;
            }
            if(!j) {
                F[i] = 0;
                break;
            }
            j = F[j];
        }
    }
    return F;
}
