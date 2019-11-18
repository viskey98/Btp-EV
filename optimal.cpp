#include<bits/stdc++.h>
using namespace std;
const int N = 1005; // Maximum no. of nodes the road network can have
vector < vector < pair< int, int> > > graph(N); // should be a simple graph
vector < int > s;
set< pair<double, int>, greater< pair<double, int > > > p;
map <int, double> demand;
const int INF = 100000;
int n, m;
int candidate[N];
int penalty_dis[N];
int roc;
double average_demand;
double alpha;
double lamda;
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

void init()
{
    cin>>n;
    int i;
    for(i = 0; i < n+5; i++) 
        penalty_dis[i] = INF;
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
    cin>>alpha;
    cin>>lamda;
}
int F(double d)
{
    // implement some function 
    double new_roc = roc;
    if( d > average_demand)
        new_roc *= average_demand/d;
    return (int)new_roc;
}
vector < int > cur;
double best_cost = INF;
double best_travel_penalty;
vector < int > out;
void dfs(int i, int d, int max_dis, int* vis)
{
    if( d > max_dis) return;
    vis[i] = 1;

    if(candidate[i])
        penalty_dis[i] = fmin(penalty_dis[i], d);

    int j, si = graph[i].size();
    for( j = 0; j < si; j++ )
    {
        pair< int, int > v = graph[i][j];
        if(vis[v.first]) continue;
        dfs(v.first, d+v.second, max_dis, vis);
    }
}
bool check_all_reachable()
{
    int i, vis[n+5] = {};
    for( i = 0; i < n; i++) penalty_dis[i] = INF;
    int si = cur.size();
    for( i = 0; i < si; i++)
    {
        int j;
        for( j = 0; j < n; j++) vis[j] = 0;
        penalty_dis[cur[i]] = 0;
        dfs(cur[i], 0, F(demand[cur[i]]), vis);
    }
    for( i = 0; i < n; i++)
    {
        if(candidate[i] && penalty_dis[i] == INF) return false;
    }
        
    return true;
}
void generate(int i)
{
    if( i == n)
    {
        if(check_all_reachable())
        {
            double tp = 0;
            double totp = 0;
            int no = cur.size();
            int j;
            for(j = 0; j < n; j++)
            {
                if(candidate[j])
                    tp += alpha*demand[j]*penalty_dis[j];
            }
            totp = lamda*tp + (1-lamda)*no;
            if(totp < best_cost)
            {
                out.clear();
                best_travel_penalty = tp;
                for( j = 0; j < no; j++)
                {
                    out.push_back(cur[j]);
                }
                best_cost = totp;
            }
        }
        return;
    }
    if(candidate[i])
    {
        cur.push_back(i);
        generate(i+1);
        cur.pop_back();
    }
    generate(i+1);
}
int main()
{
    init();
    generate(0);
    sort(out.begin(), out.end());
    cout<<"The charging point locations are: \n";
    int i;
    for(i = 0; i < out.size(); i++)
        cout<<out[i]+1<<" ";
    printf("\nIncurring costs:\nTravel penalty: %f\nNo. of charging points: %d\n", best_travel_penalty, out.size());
    cout<<"Total reduced cost: "<<best_cost<<" units\n";
}
