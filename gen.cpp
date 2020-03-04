#include<bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define FOR0(i,n) for(i=0;i<n;i++)
#define FOR(i,j,n) for(i=j;i<n;i++)
#define FORD(i,j,k) for(i=j;i>=k;i--)
#define pb push_back
#define mp make_pair
#define ff first
#define ss second
#define inf 1000000000
#define ninf -1000000000
#define endl '\n' 
#define ordered_set tree<int, null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>
// Use cout.flush() for interactive problems.
// Use this for increased stack size: g++ -o a.exe -Wl,--stack=256000000 -O2 source.cpp
inline long long  MAX2(long long  a, long long int b){return (a)>(b)?(a):(b);}
inline long long  MAX3(long long  a, long long  b,long long  c){return (a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c));}
inline long long  MIN2(long long  a, long long  b){return (a)<(b)?(a):(b);}
inline long long  MIN3(long long  a, long long b,long long c){return (a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c));}
using namespace std;
typedef long long ll;
typedef pair<int,int> ii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector< ii > vii;
vi p, r, ssi; 
void initialize(int N) 
{
        p.clear();
        r.clear();
        ssi.clear();
        r.assign(N, 0);
        ssi.assign(N, 1);
        p.assign(N, 0); for (int i = 0; i < N; i++) p[i] = i; 
}
int findSet(int i) { return (p[i] == i) ? i : (p[i] = findSet(p[i])); }
bool isSameSet(int i, int j) { return findSet(i) == findSet(j); }
void unionSet(int i, int j)
{
    if (!isSameSet(i, j))
    {
        int x = findSet(i), y = findSet(j);
        if (r[x] > r[y]) 
        {
            p[y] = x; 
            ssi[x] +=ssi[y];
        }
        else 
        {
            p[x] = y;
            ssi[y] += ssi[x];
            if (r[x] == r[y]) r[y]++;
        }
    }
}
int sizeOfSet(int i){return ssi[findSet(i)];}
int main()
{
    srand(time(NULL));
    int t = 100; // no of test cases required
    cout<<t<<endl;
    while(t--)
    {
        int n = 15 + rand()%6;
        cout<<n<<endl;
        int m = n-1, M = 50;
        int no_edges = m + rand()%(M-m+1);
        cout<<no_edges<<endl;
        initialize(n+1);
        int i = 0;
        while(i < m)
        {
            int u = 1 + rand()%n, v = 1 + rand()%n;
            if(isSameSet(u, v)) continue;
            int w = 10 + rand()%21;
            unionSet(u, v);
            cout<<u<<" "<<v<<" "<<w<<endl;
            i++;
        }
        while(i < no_edges)
        {
            int u = 1 + rand()%n, v = 1 + rand()%n;
            if(u == v) continue;
            int w = 10 + rand()%21;
            cout<<u<<" "<<v<<" "<<w<<endl;
            i++;
        }
        m = 0.4*n, M = 0.6*n;
        int no_of_candidates = m + rand()%(M-n+1);
        cout<<no_of_candidates<<endl;
        vi nodes;
        FOR0(i, n) nodes.pb(i+1);
        i = n;
        while(i > no_of_candidates)
        {
            random_shuffle(nodes.begin(), nodes.end());
            nodes.pop_back();
            i--;
        }
        FOR0(i, no_of_candidates)   cout<<nodes[i]<<" ";
        cout<<endl;
        int roc = 20 + rand()%31;
        cout<<roc<<endl;
        FOR0(i, no_of_candidates)
        {
            int demand = 2 + rand()%5;
            cout<<demand<<" ";
        }
        cout<<endl;
        double alpha = 0.2;
        cout<<alpha<<endl;
        double lamda = 0.025;
        cout<<lamda<<endl;
        int cap = 2 + rand()%7;
        cout<<cap<<endl;
    }
    cerr <<endl<< "Time elapsed : " << clock() * 1000.0 / CLOCKS_PER_SEC << " ms" << '\n';
}
