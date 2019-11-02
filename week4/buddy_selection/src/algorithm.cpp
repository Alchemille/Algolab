#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


using namespace std;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, long>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;
typedef boost::graph_traits<graph>::vertex_descriptor Vertex;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map_type;


void max_cardinality_matching(long n, long c, long f) {

    // read caracteristics
    vector<vector<string>> caracteristics(n, vector<string>(c));
    for (int i=0; i<n; i++) {
        for (int j=0; j<c; j++) {
            cin >> caracteristics[i][j];
        }
    }

    //build graph suh that edge if at leat f caracteristics in common
    graph G(n);
    for (int stud1=0; stud1<n; stud1++) {
        for (int stud2=0; stud2<n; stud2++) {
            if (stud1 != stud2) {
                int common = 0;
                for (vector<string>::iterator it1 = caracteristics[stud1].begin(); it1!=caracteristics[stud1].end(); ++it1) {
                    for (vector<string>::iterator it2 = caracteristics[stud2].begin(); it2!=caracteristics[stud2].end(); ++it2) {
                        if ((*it1).compare(*it2) == 0) {
                            common ++;
                        }                    
                    }
                }
                // constraint on graph: sticly more than f common caracteristics to make an edge
                if (common > f) {
                    // add edge
                    Vertex u = stud1;
                    Vertex v = stud2;
                    boost::add_edge(u, v, G);
                }
            }
        }
    }


    // define exterior property map to store maximum matching
    vector<Vertex> mate_map(n);
    boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

    // if the size of the matching is at least n/2, there is a better matching than proposed by the teacher
    if (matching_size >= n/2) cout << "not optimal\n";
    else cout << "optimal\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    long t, n, c, f;
    std::cin >> t;

    while(t--) {
        cin >> n >> c >> f;
        max_cardinality_matching(n, c, f);
    }
}
