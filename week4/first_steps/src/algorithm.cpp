#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


using namespace boost;
using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor Vertex;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map_type;


void build_mst(int V, int E) {


    // build graph
    weighted_graph G(V);

    // add edges and weights
    weight_map_type weights = boost::get(boost::edge_weight, G);

    for(int i=0; i<E; i++) {
        int v1, v2, w;
        cin >> v1 >> v2 >> w;
        edge_desc e = boost::add_edge(v1, v2, w, G).first;
        weights[e] = w;
    }

    // compute mst
    vector<edge_desc> mst;
    kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    // compute weight of mst
    int sum = 0;
    std::vector<edge_desc>::iterator it = mst.begin();
    for(; it != mst.end(); ++it) {
        sum += weights[*it];
    }

    // distance
    std::vector<int> dist_map(V);
    Vertex source = 0;
    boost::dijkstra_shortest_paths(G, source, boost::distance_map(
        boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G)
        )
    ));

    cout << sum << " " << *max_element(dist_map.begin(), dist_map.end()) << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t, V, E;
    std::cin >> t;

    while(t--) {
        cin >> V >> E;
        build_mst(V, E);
    }
}
