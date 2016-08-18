#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <map>

#define rep(i,a,b) for(int i=(a);i<(b);++i)

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef unsigned long long ull;
typedef vector<ull> vull;

template<typename t>
ostream& operator<<(ostream& os, const vector<t>&u) {
  rep(i,0,u.size())
    os << u[i] << ' ';
  return os;
}

string compact(vi u) {
  ostringstream os;
  rep(i,0,u.size()) os << u[i];
  return os.str();
}


/* the number of variables that our boolean function depends on */
const int n = 5;
int NN; //3**n
vector<bool> f;

ull _pow(ull a, ull b) {
  if(b % 2) {
    ull x = _pow(a, b/2);
    return x*x*a; 
  } else {
    if(b) { ull x = _pow(a, b/2); return x*x;}
    else return 1;
  }
}

map<vi, int> DP;
map<vi, int> bt;

// do dynamic programming where the states are subcubes (like "110**1*1" (coded as a vector<int> u with value 11022121)) and the values DP[u] are the smallest number of nodes needed for a decision tree computing the restriction of f to the subcube. A value of -2 means that f is constant 0 on the subcube, -3 means constant 1.
// bt[u] is the variable in the root of an optimal tree for f restricted to u
int dp(vi u) {
  map<vi,int>::iterator it = DP.find(u);
  if(it != DP.end()) return it->second;
  //try each star
  int best = (1<<28);
  bool stars = false; 
  rep(i,0,n) if(u[i] == 2) {
    stars = true;
    int p = _pow(3, i);
    vi L = u; L[i] = 0;
    vi R = u; R[i] = 1;
    int dL = dp(L), dR = dp(R);
    if(dL == dR && dL < 0) {
      return DP[u] = dL;
    }
    if(dL < 0 && dR < 0) {
      bt[u] = i;
      return DP[u] = 1;
    }
    if(dL < 0) dL = 0;
    if(dR < 0) dR = 0;
    int tmp = 1 + dL + dR;
    if(tmp < best) {
      best = tmp;
      bt[u] = i;
    }
  }
  if(stars) {
    return DP[u] = best;
  } else {
    int t = 0;
    rep(i,0,n) if(u[i]) t += (1<<i);
    DP[u] = (f[t] ? -3:-2);
    return DP[u];
  }
}

void print_rec(vi u) {
  int tmp = dp(u);
  if(tmp >= 0) {
    int i = bt[u];
    cout << "n" << compact(u) << " [shape=circle,label=\"x"<<i<<"\"]" << endl;
    vi v = u;
    v[i] = 0;
    cout << "n" << compact(u) << " -> n" << compact(v) << " [label=\"0\"]"<< endl;
    print_rec(v);
    v[i] = 1;
    cout << "n" << compact(u) << " -> n" << compact(v) << " [label=\"1\"]"<< endl;
    print_rec(v);
  } else {
    if(tmp == -2) {
      cout << "n"<<compact(u) << " [shape=rectangle,label=\"0\"]" << endl;
    } else if(tmp == -3) {
      cout << "n"<<compact(u) << " [shape=rectangle,label=\"1\"]" << endl;
    }
  }
}

/*
  print the result in the .dot file format
  on ubuntu 14.04, I do 
  >> sudo apt-get install graphviz
  >> g++ thisfile.cpp
  >> ./a.out | dot -Tpdf >foo.pdf ; evince foo.pdf ;
  to see the result.
*/
void print() {
  vi foo; rep(i,0,n) foo.push_back(2); 
  //foo is the full (sub)cube
  cout << "digraph D {" << endl;
  print_rec(foo);
  cout << "}" << endl;

}

int main() {
  srand(time(0));
  NN = _pow(3, n);

/* assign values to our boolean function f to be studied */
  f.resize(1<<n);
  rep(t,0,(1<<n))
    f[t] = rand()%2;
/* end of assignment */

/* find the smallest decision tree describing f */
  rep(u, 0, _pow(3, n)) {
    vi tmp; int v = u ;
    rep(i,0,n) { tmp.push_back(v%3); v /= 3; }
    dp(tmp);
  }
/* end of finding */

  print();

/* print the pairs (x, f(x)) */
/*
  rep(t,0,(1<<n)) {
    int u = t;
    vi tmp;
    rep(i,0,n) {
      tmp.push_back(u % 2);
      u /= 2;
    }
    cout << tmp << " : " << f[t] << endl;
  }
*/

/* debugging the dynamic programming */
/*
  string s;
  while(cin >> s) {
    vi tmp(n);
    rep(i,0,n) {
      if(s[i] == '*') {
        tmp[i] = 2;
      } else if(s[i] == '1') {
        tmp[i] = 1;
      } else {
        tmp[i] = 0;
      }
    }
    auto it = bt.find(tmp);
    cout << s << " : " <<  dp(tmp);
    if(it!=bt.end()) cout  << " (" <<(it->second)<<" )" << endl;
    else cout << endl;
  }
*/
  return 0;
}


