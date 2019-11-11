#include<bits/stdc++.h>
using namespace std;
const int N = 1005;
vector < vector < pair< int, int> > > graph(N); // should be a simple graph
vector < int > s;
set< pair<double, int>, greater< pair<double, int > > > p;
map <int, double> demand;
int candidate[N];
int roc;
double average_demand;
// input format:
// n (no of vertices)
// edges (no of edges)
// next "edges" no. of lines of the format u v w (edge between node u-v of weight w)
// m (no of candidate nodes)
// "m" space separated integers specifying the candidate nodes ( 1-indexed )
// roc (the radius of coverage)
// "m" space separated integers specifying the demand values (float) of the candidate nodes (in the same order as the candidated nodes given)

void init()
{
    int n;
    // no. of vertices
    cin>>n;
    int i;
    int edges;
    // no. of edges
    cin>>edges;
    for( i = 0; i < edges; i++)
    {
        int u ,v, w;
        cin>>u>>v>>w;
        u--;v--;
        graph[u].push_back(make_pair(v, w));
        graph[v].push_back(make_pair(u, w));
    }
    int m;
    // no. of candidate nodes
    cin>>m;
    for( i = 0; i < m; i++)
    {
        int u;
        cin>>u;
        u--;
        s.push_back(u);
        candidate[u] = 1;
    }
    cin>>roc;
    for( i = 0; i < m; i++)
    {
        cin>>demand[s[i]];
        average_demand += demand[s[i]];
        p.insert(make_pair(demand[s[i]], s[i]));
    }
    average_demand /= (double)n;
}
int F(double d)
{
    // implement some function 
    double new_roc = roc;
    if( d > average_demand)
        new_roc *= average_demand/d;
    return (int)new_roc;
}   
void dfs(int i, int d, int max_dis, int* vis)
{
    if( d > max_dis) return;
    vis[i] = 1;
    if(candidate[i])
    {
        if(p.find(make_pair(demand[i], i)) != p.end())
            p.erase(make_pair(demand[i], i));
    }
    int j, si = graph[i].size();
    for( j = 0; j < si; j++ )
    {
        pair< int, int > v = graph[i][j];
        if(vis[v.first]) continue;
        dfs(v.first, d+v.second, max_dis, vis);
    }
}
void remove_neighbourhood(int x, int d)
{
    int vis[N] = {};
    dfs(x, 0, d, vis);
    return;
}
int main()
{
    vector< int > out;
    init();
    while(p.size())
    {
        int x = p.begin()->second;
        p.erase(p.begin());
        out.push_back(x);
        remove_neighbourhood(x, F(demand[x]));
    }
    int i;
    sort(out.begin(), out.end());
    cout<<"The charging point locations are: \n";
    for(i = 0; i < out.size(); i++)
    {
        cout<<out[i]+1<<" ";
    }
    
}
