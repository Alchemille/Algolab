#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;


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

/*
Progression:

- first, had 4*n nodes: s, t, n days, n consumptions, 2n freezers
- then, s, t, n days, 2n freezers
- then s, t, n days, n freezers
- then s, t, n days and no freezers

- first, called cycle canceling
- then nonnegative by adjusting price with constant. Careful to readjust when reading flow
*/

using namespace std;

void testcase() {

    int biggest = std::numeric_limits<int>::max();
    int n, a, c, s, p, v, e, n_students = 0, max_prices = 0;
    cin >> n;
    graph G(n+2);
    edge_adder adder(G);

    int source = n;
    int sink = n+1;

    // edges source days
    for (int i=0; i<n; i++) {
        cin >> a >> c;
        adder.add_edge(source, i, a, c);
    }

    // edges day to sink: readjust prices to be nonnegative by computing max_price
    vector<int> prices(n);
    vector<int> students(n);

    for (int i=0; i<n; i++) {
        cin >> s >> p;
        max_prices = max(p, max_prices);
        n_students += s;
        prices[i] = p;
        students[i] = s;
    }
    for (int i=0; i<n; i++) {
        adder.add_edge(i, sink, students[i], -prices[i] + max_prices);
    }

    // edges from day to day
    for (int i=0; i<n-1; i++) {
        cin >> v >> e;
        adder.add_edge(i, i+1, v, e);
    }

    // // edges freezer to freezer
    // for (int i=n; i<2*n-1; i++) {
    //     cin >> v >> e;
    //     adder.add_edge(i, i+1, v, e);
    // }

    // other edges with infinite capacity and 0 cost
    // for (int i=0; i<n; i++) {
    //     adder.add_edge(i, n+i, biggest, 0); // edge from day to freezer
    //     adder.add_edge(n+i, i, biggest, 0); // edge from freezer to day 
    // }
    
    // find cycle nonnegative
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost2 = boost::find_flow_cost(G);

    // compute flow by summing full capacities over a cut. readjust profit by adding back the prices
    int s_flow = 0;
    int profit = -cost2;
    for (auto it=boost::out_edges(sink, G).first; it!=boost::out_edges(sink, G).second; ++it) {
        int flow_edge = boost::get(boost::edge_residual_capacity, G, *it) - boost::get(boost::edge_capacity, G, *it);
        s_flow += flow_edge;
        profit += flow_edge * max_prices;
        // cout << profit << " " << flow_edge << " " << max_prices << "\n";
    }

    if (s_flow < n_students) cout << "impossible ";
    else cout << "possible ";
    cout << s_flow << " " << profit << "\n";
}


int main(int argc, char const *argv[]) {

    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();

}
