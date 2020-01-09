#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;
typedef	boost::graph_traits<graph>::out_edge_iterator out_edge_it;

using namespace std;

/*
battleship planed ON astronomical objects! => add vertex edges
*/

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {

    int n, m, s, d;
    cin >> n >> m >> s >> d;
    graph G(2*n + 2);
    int source = 2*n;
    int sink = 2*n + 1;
    edge_adder adder(G);

    // set vertices edges
    for (int i = 0; i < n; i ++) {
        adder.add_edge(i, i + n, 1); // or could have set (2*i, 2*i + 1) 
    }

    // travelling edges
    for (int i = 0; i < m; i ++) {
        int u, v;
        cin >> u >> v;
        adder.add_edge(u + n, v, 1); 
    }

    // source edges
    for (int i = 0; i < s; i ++) {
        int ss;
        cin >> ss;
        adder.add_edge(source, ss, 1);
    }
    
    // sink edges
    for (int i = 0; i < d; i ++) {
        int ss;
        cin >> ss;
        adder.add_edge(ss + n, sink, 1);
    }

    int flow = boost::push_relabel_max_flow(G, source, sink);
    cout << flow << "\n";    
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
