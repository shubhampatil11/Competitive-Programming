/*
  Deterministic Finite Automaton
  Can be used in KMP Algorithm
 */

const int R = /* Enter Radix Value */;

int dfa[R][R];

void DFA(string &s)
{
    int M = s.length();
    int X = 0;
    for(int i = 0; i < R ; i++) dfa[i][0] = 0;
    dfa[s[0]-'a'][0] = 1;
    for(int i = 1; i < M ; i++)
    {
        for(int c = 0; c < R; c++) dfa[c][i] = dfa[c][X];
        dfa[s[i]-'a'][i] = i+1;
        X = dfa[s[i]-'a'][X];
    }
}
