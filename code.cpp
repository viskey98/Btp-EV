#include<bits/stdc++.h>
using namespace std;
const int N = 1005;
vector < vector < pair< int, int> > > graph(N); // should be a simple graph
vector < int > s;
set< pair<int, int>, greater< pair<int, int > > > p;
int demand[N];
int candidate[N];
int roc;

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
    for( i = 0; i < n; i++)
    {
        cin>>demand[i];
        p.insert(make_pair(demand[i], i));
    }
}
int F(int x, double d)
{
    // implement some function 
    return x;
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
        remove_neighbourhood(x, F(roc, demand[x]));
    }
    int i;
    sort(out.begin(), out.end());
    cout<<"The charging point locations are: \n";
    for(i = 0; i < out.size(); i++)
    {
        cout<<out[i]+1<<" ";
    }
    
}