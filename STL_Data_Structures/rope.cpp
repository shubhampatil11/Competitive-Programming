#include<ext/rope>
using namespace __gnu_cxx;

/**
rope <...> R
Beginning of the rope: R.mutable_begin()
Ending of the rope: R.mutable_end()
Insert: R.insert()
Access: R[idx]
Persistent Copy: R[i] = (rope<char>)R[i - 1]
**/
