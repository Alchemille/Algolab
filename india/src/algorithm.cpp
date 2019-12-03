#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

using namespace std;

/*
Strategy: binary search to find max flow that does not make cost overflow.
To add bound on flow, just 1 additional edge.

Doubts:
- what is the difference in (std::tie, boost::tie) ?
- what is the difference between boost::edge() and boost::add_edge() ?
- How to know if complexity is OK for the test duration, is there golden rule?

Careful to:
- edges() returns edge_iterators and is in namespace graph
- in binary search to find the max value that does something:
here, search the max flows that does not make cost overflow.
Record the current best legit max flow.
Left -> Leftest such that we dont know if legit
Right -> Rightest such that we know that not legit

*/

// Custom edge adder class
class edge_adder { // modified !!
 graph &G;
 bool doublon = false;

 public:

  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!

    edge_desc e; bool exists;
    boost::tie(e, exists) = boost::add_edge(from, to, G);

    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }

  // uses edge() instead of add_edge()
  void update_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::edge(from, to, G).first;
    const edge_desc rev_e = boost::edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }

};

std::pair<long, long> cost_flow_restricted(graph& G, int source, int sink) {

    // compute cost and flow with luggage as an upper bound of the flow
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    long cost = boost::find_flow_cost(G);

    long flow = 0; // symbolizes number of luggage
    for (auto it = boost::out_edges(source, G).first; it != boost::out_edges(source, G).second; ++it) {
        flow += boost::get(boost::edge_capacity, G, *it) - 
                boost::get(boost::edge_residual_capacity, G, *it); // or use maps !
    }

    return {cost, flow};
} 

void testcase() {
    int c, g, b, k, a;
    cin >> c >> g >> b >> k >> a;

    graph G(c + 1);
    edge_adder adder(G);

    for (int i = 0; i < g; i ++) {
        int x, y, d, e;
        cin >> x >> y >> d >> e;
        adder.add_edge(x, y, e, d);
    }

    // see if the min cost on the raw graph is affordable
    boost::successive_shortest_path_nonnegative_weights(G, k, a);
    long cost = boost::find_flow_cost(G);

    long flow = 0; // symbolizes number of luggage
    for (auto it = boost::out_edges(k, G).first; it != boost::out_edges(k, G).second; ++it) {
        flow += boost::get(boost::edge_capacity, G, *it) - 
                boost::get(boost::edge_residual_capacity, G, *it); // or use maps !
    }

    if (cost <= b) { 
        cout << flow << "\n";
        return;
    }

    // restrict the flow (number of luggage) to also restrict the cost. Use binary search
    long best_flow = 0;
    long best_cost = 0;
    int left = 0;
    int right = flow;
    int new_source = boost::num_vertices(G);

    adder.add_edge(new_source, k, flow, 0);

    while (left < right) { // WOULD NOT WORK WITH <= : infinite loop

        int middle = (left + right) / 2; 
        adder.update_edge(new_source, k, middle, 0); // !! important to update and not add!! use difference between edge() and add_edge()

        boost::tie(cost, flow) = cost_flow_restricted(G, new_source, a);

        if (cost > b) { // restrinct the flow more
            right = middle; // WOULD NOT WORK WITH middle + 1
        }
        else { // loosen the flow
            left = middle + 1;
            best_cost = cost;
            best_flow = flow;
        }
    }
    cout << best_flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
