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
const long INF = 2000000003;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, long>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_iterator;
typedef graph_traits<weighted_graph>::vertex_descriptor Vertex;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map_type;


void find_route(long n, long e, long s, long a, long b) {

    // define a graph for each species
    vector<weighted_graph> vector_graph_species(s, weighted_graph(e));
    weighted_graph G(n);

    long t1, t2, w;
    // fill edges and weights for each species graph
    for (long i=0; i<e; i++) {
        
        cin >> t1 >> t2;
        for (long j=0; j<s; j++) {
            cin >> w;
            edge_desc new_edge = boost::add_edge(t1, t2, w, vector_graph_species[j]).first;
        }
        edge_desc new_edge_G = boost::add_edge(t1, t2, INF, G).first;
    }

    // get mst for each species
    vector<vector<edge_desc>> mst_species(s, vector<edge_desc>());
    for (long i=0; i<s; i++) {
        boost::kruskal_minimum_spanning_tree(vector_graph_species[i], std::back_inserter(mst_species[i]));
    }

    // build final insects graph
    // edges are union of private networks
    // weights are minimum over all species' mst

    weight_map_type weights_G = boost::get(boost::edge_weight, G);
    for (long i=0; i<s; i++) {
        
        std::vector<edge_desc>::iterator it = mst_species[i].begin();
        
        for (; it != mst_species[i].end(); ++it) {
            
            // add edge to G if weight less than current weight or does not exist in G
            edge_desc edge = *it;
            Vertex u = source(edge, vector_graph_species[i]);
            Vertex v = target(edge, vector_graph_species[i]);
            edge_desc edge_G = boost::edge(u, v, G).first;

            cout << i << " " << weights_G[edge_G] << " " << weights_G[edge] <<  "\n";
            weights_G[edge_G] = min(weights_G[edge], weights_G[edge_G]);
            
        }
    }

    // DEBUG
    // auto edge_it = edges(G);
    // edge_iterator begin = edge_it.first;
    // edge_iterator end = edge_it.second;
    // for (; begin != end; ++begin) {
    //     cout << "weights " << weights_G[*begin] << "\n";
    // }

    // find best path in final graph with dijkstra
    std::vector<long> distances_final_graph(n);
    Vertex source = a;
    Vertex dest = b;
    boost::dijkstra_shortest_paths(G, source, boost::distance_map(
        boost::make_iterator_property_map(
            distances_final_graph.begin(), boost::get(boost::vertex_index, G)
        )
    ));

    cout << distances_final_graph[dest] << "\n";

    // consume hives
    long hive;
    for (long i=0; i<s; i++) {
        cin >> hive;
    }
}

int main(long argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    long t, n, e, s, a, b;
    std::cin >> t;

    while(t--) {
        cin >> n >> e >> s >> a >> b;
        find_route(n, e, s, a, b);
    }
}
