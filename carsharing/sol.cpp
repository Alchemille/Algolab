#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void solve(){
    int n, S;
    cin >> n >> S;

    graph G;
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    vertex_desc source = boost::add_vertex(G);
    vertex_desc sink = boost::add_vertex(G);

    vector<vector<vertex_desc>> v(S+1);
    for(int i = 1; i <= S; ++i){
        v[i].resize(100001, -1);
        v[i][0] = boost::add_vertex(G);
        int c;
        cin >> c;
        adder.add_edge(source, v[i][0], c, 0);
    }

    for(int i = 0; i < n; ++i){
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        if(v[s][d] == -1){
            v[s][d] = boost::add_vertex(G);
        }
        if(v[t][a] == -1){
            v[t][a] = boost::add_vertex(G);
        }
        adder.add_edge(v[s][d], v[t][a], 1, -p+(a-d)*110);
    }

    vector<int> pred(S+1);
    for(int i = 1; i <= S; ++i){
        pred[i] = 0;
    }

    for(int i = 1; i < 100001; ++i){
        for(int j = 1; j <= S; ++j){
            if(v[j][i] != -1){
                adder.add_edge(v[j][pred[j]], v[j][i], INT32_MAX, (i-pred[j])*110);
                pred[j] = i;
            }
        }
    }

    for(int j = 1; j <= S; ++j){
        adder.add_edge(v[j][pred[j]], sink, INT32_MAX, (100000-pred[j])*110);
    }

    cout << boost::num_vertices(G) << endl; // the graph has 20 003 vertices
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    long cost = boost::find_flow_cost(G);
    long s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    long flow = s_flow;
    cout << -(cost - 110*100000*flow) << "\n";
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while(t--) solve();
}
