#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

/*
after added only single loops, try one call to dijkstra, with master source
https://stackoverflow.com/questions/30832309/multi-source-single-destination-algorithm
also play with iterator indices :-)
*/ 


using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
     boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor Edge;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_iterator;
typedef boost::graph_traits<weighted_graph>::vertex_iterator vertex_iterator;

// Declare a matrix type and its corresponding property map that
// will contain the distances between each pair of vertices.
typedef boost::exterior_vertex_property<weighted_graph, int> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;

int INF = std::numeric_limits<int>::max();

void planet_delivery() {

    int n, m, k, T, u, v, w;
    cin >> n >> m >> k >> T;
    weighted_graph G(n);
    vector<bool> teleportation_indices(n, false);
    for (int i = 0; i<T; i++) {
        cin >> w;
        teleportation_indices[w] = true;
    }
    // build traditional graph
    for (int i=0; i<m; i++) {
        cin >> u >> v >> w;
        boost::add_edge(u, v, w, G);
    }
    
    // build extended graph with teleportation edges
    // compute strong connected components
    vector<int> scc_map(n); // exterior property map
    int nscc = boost::strong_components(G, boost::make_iterator_property_map(scc_map.begin(),
        boost::get(boost::vertex_index, G)));

    // add a vertex for each scc
    for (int i=0; i<nscc; i++) {
        boost::add_vertex(G); // graph has now nscc + n vertices
    }

    // compute number of teleporter connected in each scc. used for weight of teleportation edges
    vector<int> teleportation_per_nscc(nscc, 0);
    vertex_iterator it1_begin = vertices(G).first;
    vertex_iterator it1_end = vertices(G).second;
    
    for(; it1_begin!=it1_end-nscc; ++it1_begin) {

        int index = boost::get(boost::vertex_index, G, *it1_begin);
        if (teleportation_indices[index]) {
            int scc = scc_map[index];
            teleportation_per_nscc[scc] += 1;
        }
    }

    // add teleportation edges: double loop iterators vertices since want to add in both directions
    it1_begin = vertices(G).first;
    it1_end = vertices(G).second;
    for (; it1_begin!=it1_end-nscc; ++it1_begin) {
        int index = boost::get(boost::vertex_index, G, *it1_begin);
        int index_scc = n + scc_map[index];
        if (index < n && teleportation_indices[index]) {
            boost::add_edge(index, index_scc, teleportation_per_nscc[scc_map[index]]-1, G);
            boost::add_edge(index_scc, index, 0, G);
        }
    }

    // add master source
    Vertex master_source = boost::add_vertex(G);
    it1_begin = vertices(G).first;
    vertex_iterator it2_begin = vertices(G).first;
    it1_end = vertices(G).second;
    Vertex sink = *(it1_end-2-nscc);

    for(; it1_begin!=it2_begin+k; ++it1_begin) {
        boost::add_edge(master_source, *it1_begin, 0, G);
    }

    // compute all pairs shortest path with floyd warshall
    int shortest_path = INF;
    vector<int> dist_map(n+1+nscc, INF);
    boost::dijkstra_shortest_paths(G, master_source,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

    shortest_path = dist_map[n-1];

    if (shortest_path <= 1000000) cout << shortest_path << "\n";
    else cout << "no\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while (t--) planet_delivery();
}