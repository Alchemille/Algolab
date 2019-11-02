#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

/*
https://stackoverflow.com/questions/3141087/what-is-meant-with-const-at-end-of-function-declaration`
https://stackoverflow.com/questions/31540115/boost-graph-getting-adjacent-vertices
http://www.cplusplus.com/reference/vector/vector/end/
*/

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
     boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor Edge;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_iterator;
typedef boost::graph_traits<weighted_graph>::vertex_iterator vertex_iterator;
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
    
    // vertex_iterator start = boost::vertices(G).first;
    // vertex_iterator end = boost::vertices(G).second;
    // for (; start!=end; ++start) {
    //     Vertex v =*start;
    //     auto index = boost::get(boost::vertex_index, G, v);
    //     cout << index << "\n";
    // }

    // build extended graph with teleportation edges
    // compute strong connected components
    vector<int> scc_map(n); // exterior property map
    int nscc = boost::strong_components(G, boost::make_iterator_property_map(scc_map.begin(),
        boost::get(boost::vertex_index, G)));

    // compute number of teleporter UNDIRECTED EDGE in each scc. double loop iteration vertices. 
    // !! not complete double loop to not count twice, since compute number of distinct teleportation links !!
    // inner iterators defined in outer loop !!
    vector<int> teleportation_per_nscc(nscc, 0);
    vertex_iterator it1_begin = vertices(G).first;
    vertex_iterator it1_end = vertices(G).second;
    
    for(; it1_begin!=it1_end; ++it1_begin) {

        vertex_iterator it2_begin = it1_begin;
        vertex_iterator it2_end = it1_end;

        for(; it2_begin!=it2_end; ++it2_begin) {

            int index_1 = boost::get(boost::vertex_index, G, *it1_begin);
            int index_2 = boost::get(boost::vertex_index, G, *it2_begin);

            // cout << index_1 << " " << index_2 << " " << scc_map[index_1] << " " << scc_map[index_2] << " " <<
            //     teleportation_indices[index_1] << " " << teleportation_indices[index_1] << "\n";
            if (index_1 != index_2 &&
                 scc_map[index_1] == scc_map[index_2] &&
                 teleportation_indices[index_1] &&
                    teleportation_indices[index_2]) {
                
                teleportation_per_nscc[scc_map[index_1]] += 1;
            }
        }
    }

    // cout << "\n";
    // for (int i=0; i<n; i++) cout << scc_map[i] << " ";
    // cout << "\n";
    // for (int i=0; i<nscc; i++) cout << teleportation_per_nscc[i] << " " ;
    // cout << "\n";

    // add teleportation edges: double loop iterators vertices since want to add in both directions
    // !!! inner iterators should be defined insined outer loop !!!
    it1_begin = vertices(G).first;
    it1_end = vertices(G).second;

    for(; it1_begin!=it1_end; ++it1_begin) {

        vertex_iterator it2_begin = vertices(G).first;
        vertex_iterator it2_end = vertices(G).second;

        for(; it2_begin!=it2_end; ++it2_begin) {

            int index_1 = boost::get(boost::vertex_index, G, *it1_begin);
            int index_2 = boost::get(boost::vertex_index, G, *it2_begin);
            
            if (scc_map[index_1] == scc_map[index_2] &&
                 teleportation_indices[index_1] &&
                    teleportation_indices[index_2]) {
                
                boost::add_edge(*it1_begin, *it2_begin, teleportation_per_nscc[scc_map[index_1]], G);
            }
        }

    }

    // edge_iterator eit1 = edges(G).first;
    // cout << "\n";
    // for (; eit1!=edges(G).second; ++eit1) {
    //     cout << boost::get(boost::edge_weight, G, *eit1) << " " <<
    //     source(*eit1, G) << " " << target(*eit1, G) << "\n";
    // }
    // cout << "\n";

    //find shortest path with dijkstra from each warehouse source
    vertex_iterator it = vertices(G).first;
    vertex_iterator it_end = vertices(G).second;
    int shortest_path = INF;
    Vertex sink = *(it_end-1);

    for (;it!=it_end; ++it) {

        Vertex source = *it;
    
        int index = boost::get(boost::vertex_index, G, source);
        int index_sink = boost::get(boost::vertex_index, G, sink);
        if (index < k) {

            vector<int> dist_map(n, INF);
            boost::dijkstra_shortest_paths(G, source,
                boost::distance_map(boost::make_iterator_property_map(
                    dist_map.begin(), boost::get(boost::vertex_index, G))));

            //cout << index <<  " " << index_sink << " " << dist_map[boost::get(boost::vertex_index, G, sink)] << " " << shortest_path << "\n";

            // compare shortest path to shortest path from other warehouse
            if (dist_map[boost::get(boost::vertex_index, G, sink)] < shortest_path) {
                shortest_path = dist_map[boost::get(boost::vertex_index, G, sink)];
            }
        }
    } 

    if (shortest_path <= 1000000) cout << shortest_path << "\n";
    else cout << "no\n";


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while (t--) planet_delivery();
}
