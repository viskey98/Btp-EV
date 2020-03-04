#include<bits/stdc++.h>
using namespace std;
const int N = 1005; // Maximum no. of nodes the road network can have
vector < vector < pair< int, int> > > graph(N); // should be a simple graph
vector < int > s;
int demand[N] = {};
const int INF = 100000;
int n, m;
int candidate[N] = {};
int roc;
double alpha;
double lamda;
double c, capacity[N];
// input format:
// n (no of vertices)
// edges (no of edges)
// next "edges" no. of lines of the format u v w (edge between node u-v of weight w)
// m (no of candidate nodes)
// "m" space separated integers specifying the candidate nodes ( 1-indexed )
// roc (the radius of coverage)
// "m" space separated integers specifying the demand values (float) of the candidate nodes (in the same order as the candidated nodes given)
// alpha (the travel penalty coeff)
// lamda (penalty factor)
// c (capacity)

void init()
{
    cin>>n;
    int i;
    int edges;
    // no. of edges
    s.clear();
    for ( i =0 ; i <n; i++)
    {
        candidate[i] = 0;
        graph[i].clear();
        demand[i] = 0;
        capacity[i] = 0;
    }
    cin>>edges;
    for( i = 0; i < edges; i++)
    {
        int u ,v, w;
        cin>>u>>v>>w;
        u--;v--;
        graph[u].push_back(make_pair(v, w));
        graph[v].push_back(make_pair(u, w));
    }
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
        double temp;
        cin>>temp;
        demand[s[i]] = temp;
    }
    cin>>alpha;
    cin>>lamda;
    cin>>c;
    for( i = 0; i < m; i++)
        capacity[s[i]] = c;
}
int subset[N] = {};
vector< vector< pair<int, int > >  > neighbours(N);
int gap[N] = {}, demand_left[N] = {};
void dfs(int i, int d, int head, int *vis)
{
    vis[i] = 1;
    if( d > roc) return;
    if( i != head && d <= roc && subset[i])
        neighbours[head].push_back(make_pair(i, d));
    int j, si = graph[i].size();
    for( j = 0; j < si; j++)
    {
        pair< int, int > v = graph[i][j];
        if(vis[v.first]) continue;
        dfs(v.first, d+v.second, head, vis);
    }
}
double cur_num = 0, cur_penalty = 0, travel_penalty = 0, best_cost = INF;
vector< vector< int > > distribute[10][10];
vector<int> cur, out;
void precompute(int i, int left, int tot, int n)
{
    if(i == n)
    {
        if(left == 0)
            distribute[tot][n].push_back(cur);
        return;
    }
    int j;
    for(j = 0; j <= left; j++)
    {
        cur.push_back(j);
        precompute(i+1, left-j, tot, n);
        cur.pop_back();
    }
}
void make_distributions()
{
    int i, j;
    for( i = 1; i <= 10; i++)
        for( j = 1; j <= min(i, 5); j++)
            precompute(0, i, i, j);

}
void satisfy(int i)
{
    if(i == n)
    {
        double cur_cost = lamda*cur_penalty + (1-lamda)*cur_num;
        if( cur_cost < best_cost)
        {
            best_cost = cur_cost;
            travel_penalty = cur_penalty;
            int j;
            out.clear();
            for( j = 0; j < n; j++)
                if(candidate[j] && subset[j]) out.push_back(j);
        }
        return;
    }
    if(!candidate[i] || subset[i])
    {
        satisfy(i+1);
        return;
    }
    int tot = demand_left[i];
    int num = neighbours[i].size();
    if( num == 0) return;
    int j, si = distribute[tot][num].size();
    for(j = 0; j < si ; j++)
    {
        int k;
        bool is = true;
        for(k = 0; k < num ; k++)
        {
            int u = neighbours[i][k].first;
            if(gap[u] < distribute[tot][num][j][k])
                is = false;
            cur_penalty += alpha*distribute[tot][num][j][k];
            gap[u] -= distribute[tot][num][j][k];
        }
        if(is)
            satisfy(i+1);
        for(k = 0; k < num ; k++)
        {
            int u = neighbours[i][k].first;
            cur_penalty -= alpha*distribute[tot][num][j][k];
            gap[u] += distribute[tot][num][j][k];
        }
    }
}
void generate(int i)
{
    if(i == n)
    {
        int j;
        for(j = 0; j < n; j++)
        {
            gap[j] = 0;
            demand_left[j] = 0;
            neighbours[j].clear();
        }
        for( j = 0; j < n; j++)
        {
            if(!candidate[j]) continue;
            if(subset[j]) 
            {
                gap[j] = capacity[j] - demand[j];
                continue;
            }
            int vis[N] = {};
            dfs(j, 0, j, vis);
            demand_left[j] = demand[j];
        }
        satisfy(0);
        return;
    }
    if(!candidate[i])
    {
        generate(i+1);
        return;
    }
    subset[i] = 1;
    cur_num++;
    generate(i+1);
    subset[i] = 0;
    cur_num--;
    generate(i+1);
}
int main()
{
    int t;
    cin>>t;
    make_distributions();
    while(t--)
    {
        cur_num = 0, cur_penalty = 0, travel_penalty = 0, best_cost = INF;
        cur.clear();out.clear();
        init();
        generate(0);
        sort(out.begin(), out.end());
        cout<<"The charging point locations are: \n";
        int i;
        for(i = 0; i < out.size(); i++)
            cout<<out[i]+1<<" ";
        printf("\nIncurring costs:\nTravel penalty: %f\nNo. of charging points: %d\n", travel_penalty, out.size());
        cout<<"Total reduced cost: "<<best_cost<<" units\n";
    }
    
}
