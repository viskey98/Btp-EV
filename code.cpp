#include<bits/stdc++.h>
#include<math.h>
using namespace std;
const int N = 1005; // Maximum no. of nodes the road network can have
vector < vector < pair< int, int> > > graph(N); // should be a simple graph
vector < int > s;
set< pair<double, int>, greater< pair<double, int > > > p;
double demand[N] = {};
const int INF = 100000;
int n, m; 
int candidate[N];
int roc;
double alpha;
double lamda;
double c, capacity[N] = {}, uv[N] = {};
double tot_penalty = 0;
vector< pair<int, int > > roc_neighbours;
// input format:
// n (no of vertices)
// edges (no of edges)
// next "edges" no. of lines of the format u v w (edge between node u-v of length w)
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
    s.clear();
    tot_penalty = 0;
    p.clear();
    // no. of edges
    for( i = 0; i < n ; i++)
    {
        graph[i].clear();
        candidate[i] = 0;
        uv[i] = 0;
        capacity[i] = 0;
        demand[i] = 0;
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
    {
        capacity[s[i]] = c - demand[s[i]];
    }
}  
void compute_uv(int i, int d, int max_dis, int head, int* vis)
{
    if(d > max_dis) return;
    vis[i] = 1;
    if(candidate[i] && i != head)
        uv[head] += capacity[i];
    int j, si = graph[i].size();
    for( j = 0; j < si; j++ )
    {
        pair< int, int > v = graph[i][j];
        if(vis[v.first]) continue;
        compute_uv(v.first, d+v.second, max_dis, head, vis);
    }
}

void dfs(int i, int d, int max_dis, int head, int* vis)
{
    if( d > max_dis) return;
    vis[i] = 1;
    if(candidate[i] && i != head && capacity[i] > 1e-3)
        roc_neighbours.push_back(make_pair(d, i));

    int j, si = graph[i].size();
    for( j = 0; j < si; j++ )
    {
        pair< int, int > v = graph[i][j];
        if(vis[v.first]) continue;
        dfs(v.first, d+v.second, max_dis, head, vis);
    }
}

void modify_uv(int i, int d, int max_dis, int head, int* vis, double ncap)
{
    if(d > max_dis) return;
    vis[i] = 1;
    if(candidate[i] && i != head)
    {
        set< pair<double, int>, greater< pair<double, int > > >::iterator it = p.find(make_pair(uv[i], i));
        if(it != p.end())
        {
            p.erase(it);
            uv[i] += ncap - capacity[head];
            p.insert(make_pair(uv[i], i));
        }
    }
    int j, si = graph[i].size();
    for( j = 0; j < si; j++ )
    {
        pair< int, int > v = graph[i][j];
        if(vis[v.first]) continue;
        modify_uv(v.first, d+v.second, max_dis, head, vis, ncap);
    }
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        init();
        int i;
        set< int > out;
        for(i = 0; i < m; i++ )
        {
            int vis[N] = {};
            uv[s[i]] = -1*demand[s[i]];
            compute_uv(s[i], 0, roc, s[i], vis);
            out.insert(s[i]);
            p.insert(make_pair(uv[s[i]], s[i]));
        }
        while(p.size())
        {
            int x = p.begin()->second;
            if(uv[x] < 1e-3) break;
            p.erase(p.begin());
            out.erase(x);
            int vis[N] = {};
            modify_uv(x, 0, roc, x, vis, 0);
            capacity[x] = 0;
            roc_neighbours.clear();
            memset(vis, 0, sizeof vis);
            dfs(x, 0, roc, x, vis);
            sort(roc_neighbours.begin(), roc_neighbours.end());
            int j, si = roc_neighbours.size();
            double dem = demand[x];
            for( j = 0; j < si; j++ )
            {
                int nei = roc_neighbours[j].second;
                double dis = roc_neighbours[j].first;
                if(dem < 1e-3) break;
                double cur = fmin(dem, capacity[nei]);
                dem -= cur;
                tot_penalty += alpha*cur*dis;
                int vis[N] = {};
                modify_uv(nei, 0, roc, nei, vis, capacity[nei]-cur);
                capacity[nei] -= cur;
            }
            memset(vis, 0, sizeof vis);
        }
        int no_of_charging_points = out.size();
        cout<<"The charging point locations are: \n";
        set< int >::iterator it = out.begin();
        while(it != out.end())
        {
            cout<<*it+1<<" ";
            it++;
        }
        cout<<endl;
        printf("\nIncurring costs:\nTravel penalty: %f\nNo. of charging points: %d\n", tot_penalty, no_of_charging_points);
        double cost = lamda*tot_penalty + (1 - lamda)*no_of_charging_points;
        cout<<"Total reduced cost: "<<cost<<" units\n";
    }
}
