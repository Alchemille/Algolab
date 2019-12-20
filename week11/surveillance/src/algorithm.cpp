#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <bits/stdc++.h> // !! necessary for min priority queue !!
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <tuple>

using namespace std;

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

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
    c_map[rev_e] = 0; // the edges are undirected
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }

  void increment_edge(int from, int to) {

    cerr << "ADD " << from << " " << to << "\n";
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);

    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;

    c_map[e] ++;
    c_map[rev_e] = 0; // the edges are undirected
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }

};
 
struct inter {
    int num_stations;
    int num_photos;
};


void testcase() {

    int n, m, k, l;
    cin >> n >> m >> k >> l;

    // initialize intersections map
    vector<inter> intersections(n);
    for (int i = 0; i < n; i ++) {
        intersections[i] = {0, 0};
    }

    for (int i = 0; i < k; i ++) {
        int inter;
        cin >> inter;
        intersections[inter].num_stations  ++;
    }

    for (int i = 0; i < l; i ++) {
        int inter;
        cin >> inter;
        intersections[inter].num_photos ++;
    }

    // build mirorring graph: 1 vertex per intersection for each mirror side

    graph G(2 * n + 2);
    edge_adder adder(G);

    // street edges
    for (int i = 0; i < m; i ++) {
        int x, y;
        cin >> x >> y;

        adder.add_edge(x, y, INT_MAX); // street in recto city
        adder.add_edge(n + x, n + y, 1); // street in verso city. Remember that all streets are sens unique        
        
    }

    // sources and sink edges
    for (int i = 0; i < n; i ++) {
        if (intersections[i].num_stations > 0) {
            adder.add_edge(2 * n, i, intersections[i].num_stations); // source edge in recto city
            adder.add_edge(n + i, 2 * n + 1, intersections[i].num_stations); // sink edge in verso city
        }
    }

    // eges between the 2 mirrors
    auto c_map = boost::get(boost::edge_capacity, G);
    for (int i = 0; i < n; i ++) {
        if (intersections[i].num_photos > 0) {
            adder.add_edge(i, n + i, intersections[i].num_photos);            
        }
    }

    // solve max flow 
    long max_flow = boost::push_relabel_max_flow(G, 2 * n, 2 * n + 1);
    cout << max_flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
