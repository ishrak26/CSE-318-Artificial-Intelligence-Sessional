#include <bits/stdc++.h>
// ordered_set
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

#define FASTIO               \
    ios::sync_with_stdio(0); \
    cin.tie(0); \
    cout.tie(0);

// #define endl '\n'

template<class T> using ordered_set =tree<T, null_type, less<T>, rb_tree_tag,tree_order_statistics_node_update>;

typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef pair<int, int> pii;
typedef pair<int, ll> pil;
typedef pair<ll, int> pli;
typedef pair<ll, ll> pll;
typedef vector<pii> vpii;
typedef vector<pil> vpil;
typedef vector<pli> vpli;
typedef vector<pll> vpll;
typedef vector<vi> vvi;

const ll INF = (1LL<<59);
const int MAXN = 5005;
const int ITR_CNT = 10;

// 0 <= alpha <= 100
void semi_greedy_maxcut(vector<pair<ll, pii>> &edges, vector<vpli> &adj, ll alpha, vi &S, vi &Sc, ll &cut) {
    int n = adj.size() - 1; // no. of nodes
    
    ll w_min = edges.back().first;
    ll w_max = edges[0].first;

    ll mu = w_min * 100LL + alpha * (w_max - w_min);
    
    vector<pair<ll, pii>> RCL_edges;
    for (auto edge : edges) {
        if (edge.first * 100LL >= mu) {
            RCL_edges.push_back(edge);
        }
        else {
            break; // sorted edge list
        }
    } 

    bitset<MAXN> X, Y; // desired sets
    X = 0, Y = 0;
    ordered_set<int> rem; // remaining vertices
    for (int i = 1; i <= n; i++) {
        rem.insert(i);
    }

    {
        int pick_idx = 0;
        // select edge at random
        if (!RCL_edges.empty()) {
            pick_idx = rand() % RCL_edges.size();
        }
        
        auto edge = edges[pick_idx];
        X.set(edge.second.first);
        Y.set(edge.second.second);
        rem.erase(edge.second.first);
        rem.erase(edge.second.second);
    }

    while (!rem.empty()) {
        vll sigma_x(n+1), sigma_y(n+1);
        ll sigma_x_min = INF, sigma_x_max = -INF;
        ll sigma_y_min = INF, sigma_y_max = -INF;
        for (auto v : rem) {
            // find sigma_x[v] and sigma_y[v]
            for (auto [w,u] : adj[v]) {
                if (Y[u]) {
                    // u belongs to Y
                    sigma_x[v] += w;
                }
                else if (X[u]) {
                    // u belongs to X
                    sigma_y[v] += w;
                }
            }

            sigma_x_min = min(sigma_x_min, sigma_x[v]);
            sigma_x_max = max(sigma_x_max, sigma_x[v]);

            sigma_y_min = min(sigma_y_min, sigma_y[v]);
            sigma_y_max = max(sigma_y_max, sigma_y[v]);
        }
        ll w_min = min(sigma_x_min, sigma_y_min);
        ll w_max = max(sigma_x_max, sigma_y_max);
        ll mu = w_min * 100LL + alpha * (w_max - w_min);

        mu = min(mu, w_max * 100LL);

        vi RCL_vertices;
        for (auto v : rem) {
            if ((max(sigma_x[v], sigma_y[v]) * 100LL) >= mu) {
                RCL_vertices.push_back(v);
            }
        }
        assert(!RCL_vertices.empty());
        int pick_idx = rand() % RCL_vertices.size();
        auto vertex = RCL_vertices[pick_idx];

        if (sigma_x[vertex] > sigma_y[vertex]) {
            // put vertex in X
            X.set(vertex);
        }
        else {
            Y.set(vertex);
        }
        rem.erase(vertex);
    }

    cut = 0;
    
    for (int i = 1; i <= n; i++) {
        if (Y[i]) {
            Sc.push_back(i);
        }
        else if (X[i]) {
            S.push_back(i);
            for (auto [w,u] : adj[i]) {
                if (Y[u]) {
                    cut += w;
                }
            }
        }
        else {
            assert(false);
        }
    }
}

// returns no. of local search iterations
int local_search_maxcut(vi &S, vi &Sc, vector<vpli> &adj, ll &cut) {
    int n = adj.size() - 1;
    
    bool change = true;
    bitset<MAXN> X;
    X = 0;

    for (auto v : S) {
        X.set(v);
    }

    // X[v] = 1 --> v in S, else v in Sc

    // calculate sigma

    int itr_cnt = 0;

    while (change) {
        itr_cnt++;
        change = false;
        for (int v = 1; v <= n && !change; v++) {
            ll sigma_s = 0, sigma_sc = 0;
            for (auto [w,u] : adj[v]) {
                if (X[u]) {
                    sigma_sc += w;
                }
                else {
                    sigma_s += w;
                }
            }
            
            if (X[v] && (sigma_sc > sigma_s)) {
                X.reset(v);
                change = true;
            }
            else if (!X[v] && (sigma_s > sigma_sc)) {
                X.set(v);
                change = true;
            }
        }
    }

    // recalculate S, Sc, cut
    S.clear();
    Sc.clear();
    cut = 0;
    for (int v = 1; v <= n; v++) {
        if (X[v]) {
            S.push_back(v);
            for (auto [w,u] : adj[v]) {
                if (!X[u]) {
                    cut += w;
                }
            }
        }
        else {
            Sc.push_back(v);
            // cut already calculated by other end in S of its edges
        }
    }

    return itr_cnt;
}

// type = (0,1,2) --> (greedy, semi-greedy, random)
ll grasp_maxcut(int itr_cnt, int type, vector<pair<ll, pii>> &edges, vector<vpli> &adj, int &ls_itr, ll &ls_best_val) {
    ll cut = -INF;
    ls_itr = 0;
    ll best_val_sum = 0;
    for (int i = 1; i <= itr_cnt; i++) {
        vi S, Sc;
        ll alpha;
        if (type == 0) {
            alpha = 100;
        }
        else if (type == 2) {
            alpha = 0;
        }
        else {
            alpha = rand() % 101;
        }
        ll w;
        semi_greedy_maxcut(edges, adj, alpha, S, Sc, w);
        ls_itr += local_search_maxcut(S, Sc, adj, w);
        best_val_sum += w;
        cut = max(cut, w);
    }
    ls_itr /= itr_cnt;
    ls_best_val = best_val_sum / itr_cnt;
    return cut;
}

int main() {
    // FASTIO

    // freopen("debug.txt", "w", stderr);

    string filepath = "set1/";

    ofstream fout;
    string filename = "out.csv";

    fout.open(filename, ios_base::out);
    fout << "Problem,,,Constructive algorithm,,Local search,,GRASP" << endl;
    fout << "Name,n,m,Randomized-1,Greedy-1,Local-1,,GRASP-1" << endl;
    fout << ",,,,,No. of iterations,Best value,No. of iterations,Best value" << endl;

    filename = "semi-greedy.csv";

    for (int filenum = 11; filenum <= 11; filenum++) {
        ifstream fin;
        string filename = filepath + "g" + to_string(filenum) + ".rud";
        fin.open(filename, ios_base::in);

        int n, m;
        fin >> n >> m;
        vector<pair<ll, pii>> edges(m); 
        vector<vpli> adj(n+1);
        for (int i = 0; i < m; i++) {
            int u, v;
            ll w;
            fin >> u >> v >> w;
            edges[i] = {w, {u,v}};
            adj[u].push_back({w, v});
            adj[v].push_back({w, u});
        }

        sort(edges.rbegin(), edges.rend());

        fout << "G" << filenum << "," << n << "," << m << ",";

        int ls_sample_cnt = 0;
        int ls_itr;
        ll ls_best_val;
        int ls_itr_sum = 0;
        ll ls_best_val_sum = 0;
        ll best_val = -INF;

        // simple randomized
        {
            int sample_cnt = 10;
            ll sum = 0;
            for (int i = 0; i < sample_cnt; i++) {
                
                ll cut = grasp_maxcut(ITR_CNT, 2, edges, adj, ls_itr, ls_best_val);
                sum += cut;
                ls_sample_cnt++;
                ls_itr_sum += ls_itr;
                ls_best_val_sum += ls_best_val;

                best_val = max(best_val, cut);
            }
            ll avg = (sum / sample_cnt);
            fout << avg << ",";
        }

        // greedy
        {
            ll cut = grasp_maxcut(ITR_CNT, 0, edges, adj, ls_itr, ls_best_val);
            ls_sample_cnt++;
            ls_itr_sum += ls_itr;
            ls_best_val_sum += ls_best_val;

            best_val = max(best_val, cut);

            fout << cut << ",";
        }
        
        fout << (ls_itr_sum / ls_sample_cnt) << "," << (ls_best_val_sum / ls_sample_cnt) << ",";
        fout << ITR_CNT << "," << best_val << endl;
        
    }

    return 0;
}