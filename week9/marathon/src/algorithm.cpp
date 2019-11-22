#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>
#include <bits/stdc++.h> // !! necessary for min priority queue !!
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <unordered_map>
#include <boost/functional/hash.hpp>

/*
First idea:
Thought we could barrer les routes and choose the lenght of the race.
Then, compute all lenghts possible s->t. For each possible lenght , compute a max flow.
Then take the max of all max flows

But actually, its not possible. The lenght of the race if the lenght of the shortet path.
Compute shortest path lenght (dijksta) then determine if an edge is path of one.
To do so, compute dijkstra both from s and from t. Very habile.

Very tricky:
Possibiliry of several streets between (u, v). Careful to only account for shortest streets.
If several shortests streets, sum capacities
*/

using namespace std;

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property<boost::edge_weight_t, int>> > > >	graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity, long distance) {

    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);

    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;

    c_map[e] = capacity;
    c_map[rev_e] = capacity; // the edges are undirected
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = distance;
    w_map[rev_e] = distance;
  }
};

void testcase() {
    int n, m, s, f;
    cin >> n >> m >> s >> f;

    // build the graph, with undirected edges
    graph G(n);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);

    unordered_map<pair<int, int>, pair<int, int>, boost::hash<pair<int, int>>> widths;
    for (int i=0; i<m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        adder.add_edge(a, b, 0, d); // for now, all capacities to 0. Only edges part of a shortest path will have a capacity
        
        if (widths.count({a, b}) == 0) {
            widths[{a, b}] = {c, d};
            widths[{b, a}] = {c, d};
        }
        else{
            // if several edges between a and b, make sure we record only the shortest
            // also, if the distance is the same, add capacity
            if(d < widths[{a, b}].second) {
                widths[{a, b}] = {c, d};
                widths[{b, a}] = {c, d};
            }
            else if (d == widths[{a, b}].second) {
                widths[{a, b}].first += c;
                widths[{b, a}].first += c;
            }
        }

    }

    // run dijkstra from s
    std::vector<int> dist_map_s(n);
    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map_s.begin(), boost::get(boost::vertex_index, G))));

    // run dijkstra from t
    std::vector<int> dist_map_t(n);
    boost::dijkstra_shortest_paths(G, f,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map_t.begin(), boost::get(boost::vertex_index, G))));

    int shortest_path_len = dist_map_s[f];

    // find vertices part of a shortest path
    vector<int> interesting_vertices;
    for (auto it = boost::vertices(G).first; it != boost::vertices(G).second; ++it) {
        int v = (*it);
        if (dist_map_s[v] + dist_map_t[v] == shortest_path_len) interesting_vertices.push_back(v); 
    }

    // find edges part of a shortest path
    for (auto it = interesting_vertices.begin(); it != interesting_vertices.end(); ++it) {

        int u = *it;
        int d_us = dist_map_s[u];
        int d_uf = dist_map_t[u];

        for (auto it_e = boost::out_edges(u, G).first; it_e != boost::out_edges(u, G).second; ++it_e) {

            int d_e = boost::get(boost::edge_weight, G, *it_e);
            int v = boost::target(*it_e, G);
            int d_vs = dist_map_s[v];
            int d_vf = dist_map_t[v];

            if (d_e + d_us + d_vf == shortest_path_len) { // trick: only put capacity in one way of the edge. All vertices on the way are visited anyway.

                // e is part of a shortest path
                //cout << u << " " << v << " " << widths[{u, v}] << "\n";
                c_map[boost::edge(u, v, G).first] = widths[{u, v}].first;
                // c_map[boost::edge(v, u, G).first] = widths[{u, v}];
            }
        }        
    }

    // compute max flow
    long max_flow = boost::push_relabel_max_flow(G, s, f);
    cout << max_flow << "\n";

    // for (auto it = boost::edges(G).first; it != boost::edges(G).second; ++it) {
    //     cout << c_map[*it] << " " << c_map[*it] - boost::get(boost::edge_residual_capacity, G, *it) << " " << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
    // }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
