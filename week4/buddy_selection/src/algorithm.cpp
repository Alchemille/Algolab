#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

/*
map::count is in linear time
complexity of edmunds: in O(n.m) (n vertices, m edges)
correction: sliding window, quite interesting
*/

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_weight_t, long>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;
typedef boost::graph_traits<graph>::vertex_descriptor Vertex;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map_type;


void max_cardinality_matching(long n, long c, long f) {

    // read caracteristics
    // fill map (string -> vector of students having the stirng as char)
    vector<vector<string>> caracteristics(n, vector<string>(c));
    unordered_map<string, vector<int>> char_to_studs;
    string tmp;
    for (int i=0; i<n; i++) {
        for (int j=0; j<c; j++) {
            cin >> tmp;
            char_to_studs[tmp].push_back(i);
            caracteristics[i][j] = tmp;
        }
    }

    // fill map (pair(stud1, stud2) -> number of characteristics in common)
    unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> commons;
    for (auto studs_1_char : char_to_studs) {

        vector<int> students_1_characteristic = studs_1_char.second;

        for (int i = 0; i < students_1_characteristic.size(); i ++) {
            int stud1 = students_1_characteristic[i];
            for (int j = i + 1; j < students_1_characteristic.size(); j ++) {
                int stud2 = students_1_characteristic[j];
                if (commons.count({stud1, stud2}) == 0) {
                    commons[{stud1, stud2}] = 1;
                }
                else commons[{stud1, stud2}] ++;
             }
        }
    }

    graph G(n);
    for (auto paire_commons : commons) {

        int stud1 = paire_commons.first.first;
        int stud2 = paire_commons.first.second;

        if (paire_commons.second > f) {
            // add edge
            boost::add_edge(stud1, stud2, G);

            
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
