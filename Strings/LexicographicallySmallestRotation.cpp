int LexicographicallySmallestRotation(const string &s) {
    int n = (int)s.length();
    int i = 0, j = 1;
    string S = s + s;
    while(i < n && j < n) {
        int k = 0;
        while(k < n && S[i + k] == S[j + k]) {
            k++;
        }
        if(S[i + k] <= S[j + k]) {
            j += k + 1;
        }
        else {
            i += k + 1;
        }
        if(i == j) {
            j++;
        }
    }
    return i < n ? i : j;
}
