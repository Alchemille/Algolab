#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

// Includes
// ========
#include <iostream>
// BGL includes
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

void testcase() {
    
    int n, m;
    cin >> n >> m;
    graph G(n + 2);
    edge_adder adder(G);

    // add source edges, sink edges, edges between each step
    for (int i = 0; i < n - 1; i ++) {

        int c;
        cin >> c;

        adder.add_edge(i, i + 1, c, 0); // step edge
        adder.add_edge(n, i, c, 0); // source edge
        adder.add_edge(i + 1, n + 1, c, 0); // sink edge
    }

    // add food items edges
    for (int i = 0; i < m; i ++) {
        
        int a, b, d;
        cin >> a >> b >> d;

        adder.add_edge(a, b, 1, -d);
    }

    // compute min cost max flow
    int flow1 = boost::push_relabel_max_flow(G, n, n + 1);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    // std::cout << "-----------------------" << "\n";
    // std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << "\n";
    // std::cout << "flow " << flow1 << "\n"; // 5
    std::cout << -cost1 << "\n"; // 12


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
