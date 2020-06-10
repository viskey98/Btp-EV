// check optimal solution about not distributing atall - some error
#include<bits/stdc++.h>
using namespace std;
const int N = 1005; // Maximum no. of nodes the road network can have
vector < vector < pair< int, int> > > graph(N); // should be a simple graph
vector < int > s;
double demand[N] = {};
const int INF = 100000;
int n, m; 
int candidate[N], grid[N];
int roc;
double alpha, beta;
double l1, l2;
double c, capacity[N] = {}, uv[N] = {};
double tot_penalty = 0;
// input format:
// t (no of test cases)
// n (no of vertices)
// edges (no of edges)
// next "edges" no. of lines of the format u v w (edge between node u-v of length w)
// m (no of candidate nodes)
// "m" space separated integers specifying the candidate nodes ( 1-indexed )
// roc (the radius of coverage)
// "m" space separated integers specifying the demand values (float) of the candidate nodes (in the same order as the candidated nodes given)
// "m" space separated integers specifying the index of the grid (1-indexed) to which the i_th candidate node is connected
// alpha (the travel penalty coeff)
// beta (the grid penalty coeff)
// l1 l2 (penalty factors)
// c (capacity)
int subset[N] = {};
vector< vector< pair<int, int > >  > neighbours(N);
int gap[N] = {}, demand_left[N] = {};
void init()
{
    cin>>n;
    int i;
    int edges;
    s.clear();
    tot_penalty = 0;
    // no. of edges
    for( i = 0; i < n ; i++)
    {
        graph[i].clear();
        candidate[i] = 0;
        uv[i] = 0;
        capacity[i] = 0;
        demand[i] = 0;
        subset[i] = 0;
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
    for( i = 0; i< m; i++ )
    {
        int g;
        cin>>g;
        g--;
        grid[i] = g;
    }
    cin>>alpha;
    cin>>beta;
    cin>>l1>>l2;
    cin>>c;
    for( i = 0; i < m; i++)
    {
        capacity[s[i]] = c - demand[s[i]];
    }
    double eps;
    cin>>eps; // dummy
}  
void dfs(int i, double d, int head, int *vis)
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
double cur_num = 0, cur_penalty = 0, travel_penalty = 0, best_cost = INF, grid_penalty = 0;
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
        int j;
        map< int, double> c;
        map< int, double>::iterator it;
        for(j = 0; j < n; j++)
            if(candidate[j] && subset[j]) c[grid[j]]++;
        double gp = 0;
        it = c.begin();
        while(it != c.end())
        {
            double cur = it->second;
            gp += beta*cur*cur*cur;
            it++;
        }
        double cur_cost = l1*cur_penalty + l2*cur_num + (1-l1-l2)*gp;
        cout<<cur_cost<<endl;
        if( cur_cost < best_cost)
        {
            best_cost = cur_cost;
            grid_penalty = gp;
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
    if(!num)
    {
        return;
    }
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
        // for( j = 0; j < n; j++)
        // {
        //     cout<<j<<" "<<neighbours[j].size()<<endl;
        // }
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
        cur_num = 0, cur_penalty = 0, travel_penalty = 0, best_cost = INF; grid_penalty=0;
        cur.clear();out.clear();
        init();
        generate(0);
        sort(out.begin(), out.end());
        cout<<"The charging point locations are: \n";
        int i;
        for(i = 0; i < out.size(); i++)
            cout<<out[i]+1<<" ";
        printf("\nIncurring costs:\nTravel penalty: %f\nGrid penalty: %f\nNo. of charging points: %d\n", travel_penalty, grid_penalty,out.size());
        cout<<"Total reduced cost: "<<best_cost<<" units\n";
    }
    cerr <<endl<< "Time elapsed : " << clock() * 1000.0 / CLOCKS_PER_SEC << " ms" << '\n';
    
}
