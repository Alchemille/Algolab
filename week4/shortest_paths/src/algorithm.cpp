#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

/*
To access to map of edge_index or vertex_index, add it to definition of graph in typedef
*/

using namespace boost;
using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_weight_t, int, 
    boost::property<edge_index_t, int>>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor Vertex;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map_type;



int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n, m, q;
    cin >> n >> m >> q;

    vector<pair<int, int>> coords(n);
    for (int i=0; i<n; i++) {
        int lon, lat;
        cin >> coords[i].first >> coords[i].second;
    }   

    // create graph
    weighted_graph G(n);
    int u, v, c;
    for (int i=0; i<m; i++) {
        cin >> u >> v >> c;
        boost::add_edge(u, v, c, G);
    }

    // treat queries
    Vertex s, t;
    for (int i=0; i<q; i++) {
        
        std::vector<long> distances_final_graph(n, std::numeric_limits<long>::max());
        std::vector<Vertex> pred_map(n);
        std::vector<Vertex> path;

        cin >> s >> t;

        boost::dijkstra_shortest_paths(G, s,
            boost::distance_map(boost::make_iterator_property_map(
                distances_final_graph.begin(), boost::get(boost::vertex_index, G)))
                    .predecessor_map(boost::make_iterator_property_map(
                        pred_map.begin(), boost::get(boost::vertex_index, G))));
        
        if (distances_final_graph[t] == std::numeric_limits<long>::max()) cout << "unreachable\n";
        else { 
            cout << distances_final_graph[t] << "\n";
            
            // display edges of path for visualization, from t to s
            Vertex current = t;
            while(s != current) {
                path.push_back(current);
                current = pred_map[current];
            }

            property_map<weighted_graph, edge_index_t>::type e_map = boost::get(boost::edge_index, G);
            std::reverse(path.begin(), path.end());
            cout << path.size() << " ";
            for (auto it=path.begin(); it!=path.end() -1 ; ++it) {
                edge_desc e = edge(*it, *(it+1), G).first;
                cout << e_map[e] << " ";
            }
            cout << "\n";
        }

    }
}
