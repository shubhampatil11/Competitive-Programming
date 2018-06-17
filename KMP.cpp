/* 
  KMP Algorithm
*/

int F[N];

void build_failure_function(string &pattern)
{
    int m = pattern.length();
    F[0] = F[1] = 0;
    for(int i = 2; i <= m; i++)
    {
        int j = F[i - 1];
        while(1)
        {
            if(pattern[j] == pattern[i - 1]) { F[i] = j + 1; break; }
            if(j == 0) { F[i] = 0; break; }
            j = F[j];
        }
    }
    return;
}

int KMP(string &text, string &pattern)
{
    int n = text.length(), m = pattern.length();
    build_failure_function(pattern);
    int i = 0, j = 0;
    while(1)
    {
        if(j == n) break;
        if(text[j] == pattern[i])
        {
            i++; j++;
            if(i == m) return j - m;
        }
        else if(i > 0) i = F[i];
        else j++;
    }
    return -1;
}
