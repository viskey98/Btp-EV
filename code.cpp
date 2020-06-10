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
int candidate[N], grid[N], grid_num[N];
vector< vector< int > > grid_con(N);
int roc;
double alpha, beta;
double l1, l2; // Please make sure  0 <= l1 + l2 <= 1
double c, capacity[N] = {}, uv[N] = {};
double travel_penalty = 0;
double eps;
vector< pair<int, int > > roc_neighbours;
// input format:
// t (no of test cases)
// n (no of vertices)
// edges (no of edges)
// next "edges" no. of lines of the format u v w (edge between node u-v of length w) all in 1-indexed format
// m (no of candidate nodes)
// "m" space separated integers specifying the candidate nodes ( 1-indexed )
// roc (the radius of coverage)
// "m" space separated integers specifying the demand values (float) of the candidate nodes (in the same order as the candidated nodes given)
// "m" space separated integers specifying the index of the grid (1-indexed) to which the i_th candidate node is connected
// alpha (the travel penalty coeff)
// beta (the grid penalty coeff)
// l1 l2 (penalty factors)
// c (capacity)
// epsilon (randomness coeff )

void init()
{
    cin>>n;
    int i;
    int edges;
    s.clear();
    travel_penalty = 0;
    p.clear();
    // no. of edges
    for( i = 0; i < n ; i++)
    {
        graph[i].clear();
        candidate[i] = 0;
        uv[i] = 0;
        capacity[i] = 0;
        demand[i] = 0;
        grid_num[i] = 0;
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
        grid_con[g].push_back(i);
    }
    cin>>alpha;
    cin>>beta;
    cin>>l1>>l2;
    cin>>c;
    for( i = 0; i < m; i++)
    {
        capacity[s[i]] = c - demand[s[i]];
    }
    cin>>eps;
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
    return;
}

void modify_grid(int g)
{
    int i, si = grid_con[g].size();
    double nval = grid_num[g]+1;
    set< pair<double, int>, greater< pair<double, int > > >::iterator it;
    for( i = 0; i < si; i++)
    {
        int u = grid_con[g][i];
        it = p.find(make_pair(uv[u], u));
        uv[u] -= beta*grid_num[g];
        uv[u] += beta*nval;
        if( it != p.end())
        {
            p.erase(it);
            p.insert(make_pair(uv[u], u));
        }
    }
    grid_num[g] = nval;
    return; 
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
            uv[i] += alpha*(ncap - capacity[head]);
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
bool is_random()
{
    double num = rand()%100 + 1;
    num = 1/num;
    if(num < eps) return true;
    return false;
}
double compute_grid_penalty(set< int > out)
{
    double cost = 0;
    map< int, double> c;
    map<int, double>::iterator it2;
    set<int>::iterator it = out.begin();
    while(it != out.end())
    {
        int cur = *it;
        c[grid[cur]]++;
        it++;
    }

    // for now assuming y = beta*x^3
    it2 = c.begin();
    while(it2 != c.end())
    {
        double cur = it2->second;
        cost += (double)beta*cur*cur*cur;
        it2++;
    }
    return cost;
}
int main()
{
    int t;
    cin>>t;
    srand(time(NULL));
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
            uv[s[i]] *= alpha;
            p.insert(make_pair(uv[s[i]], s[i]));
        }
        while(p.size())
        {
            int x;
            if(is_random())
            {
                set< pair<double, int>, greater< pair<double, int > > >::iterator it;
                it = p.begin();
                int cnt = rand()%p.size();
                i = 0;
                while(i < cnt)
                {
                    it++;
                    i++;
                }
                x = it->second;
                p.erase(it);
            }
            else 
            {
                x = p.begin()->second;
                p.erase(p.begin());
            }
            if(uv[x] < 1e-3) break;
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
                travel_penalty += alpha*cur*dis;
                int vis[N] = {};
                p.erase( make_pair(uv[nei], nei) );
                modify_grid( grid[nei] );
                modify_uv(nei, 0, roc, nei, vis, capacity[nei]-cur);
                capacity[nei] -= cur;
            }
            memset(vis, 0, sizeof vis);
        }
        int no_of_charging_points = out.size();
        double grid_penalty = compute_grid_penalty(out);
        cout<<"The charging point locations are: \n";
        set< int >::iterator it = out.begin();
        while(it != out.end())
        {
            cout<<*it+1<<" ";
            it++;
        }
        cout<<endl;
        printf("Incurring costs:\nTravel penalty: %f\nGrid penalty: %f\nNo. of charging points: %d\n", travel_penalty, grid_penalty,no_of_charging_points);
        double cost = l1*travel_penalty + l2*no_of_charging_points + (1 - l1 - l2)*grid_penalty;
        cout<<"Total reduced cost: "<<cost<<" units\n";
        cout<<endl;
    }
    cerr <<endl<< "Time elapsed : " << clock() * 1000.0 / CLOCKS_PER_SEC << " ms" << '\n';
}
