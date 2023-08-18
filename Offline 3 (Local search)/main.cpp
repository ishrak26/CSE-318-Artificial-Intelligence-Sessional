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

#define endl '\n'

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

// 0 <= alpha <= 100
void semi_greedy_maxcut(vector<pair<ll, pii>> edges, vector<vpli> adj, ll alpha, vi &S, vi &Sc, ll &cut) {
    int n = adj.size() - 1; // no. of nodes
    
    sort(edges.rbegin(), edges.rend());
    ll w_min = edges.back().first;
    ll w_max = edges[0].first;

    ll mu = w_min * 100LL + alpha * (w_max - w_min);
    
    vector<pair<ll, pii>> RCL_edges;
    for (auto edge : edges) {
        if (edge.first * 100LL >= mu) {
            RCL_edges.push_back(edge);
        }
    } 
    assert(!RCL_edges.empty());

    bitset<MAXN> X, Y; // desired sets
    X = 0, Y = 0;
    ordered_set<int> rem; // remaining vertices
    for (int i = 1; i <= n; i++) {
        rem.insert(i);
    }

    {
        int pick_idx = 0;
        // select edge at random
        pick_idx = rand() % RCL_edges.size();
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
                    sigma_x_min = min(sigma_x_min, sigma_x[v]);
                    sigma_x_max = max(sigma_x_max, sigma_x[v]);
                }
                else if (X[u]) {
                    // u belongs to X
                    sigma_y[v] += w;
                    sigma_y_min = min(sigma_y_min, sigma_y[v]);
                    sigma_y_max = max(sigma_y_max, sigma_y[v]);
                }
            }
        }
        ll w_min = min(sigma_x_min, sigma_y_min);
        ll w_max = max(sigma_x_max, sigma_y_max);
        ll mu = w_min * 100LL + alpha * (w_max - w_min);
        vi RCL_vertices;
        for (auto v : rem) {
            if (max(sigma_x[v], sigma_y[v]) * 100LL >= mu) {
                RCL_vertices.push_back(v);
            }
        }
        int pick_idx = random() % RCL_vertices.size();
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
        if (X[i]) {
            S.push_back(i);
        }
        else if (Y[i]) {
            Sc.push_back(i);
        }
        else {
            assert(false);
        }
    }
    
    for (auto edge : edges) {
        auto [u,v] = edge.second;
        if ((X[u] && Y[v]) || (Y[u] && X[v])) {
            cut += edge.first;
        }
    }
}

int main() {
    FASTIO
    
    return 0;
}