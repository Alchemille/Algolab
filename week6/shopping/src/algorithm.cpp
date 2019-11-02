#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

/*
https://stackoverflow.com/questions/11277820/bgl-adding-an-edge-with-multiple-properties
*/

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

using namespace std;


// function from slides.
void add_edge(int from, int to, long capacity, graph& G) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = capacity; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}


void shop() {

    int n, m, s, pos1, pos2;
    cin >> n >> m >> s;

    graph G(1);
    // shops and edges (shop, sink)
    for (int i=0; i<s; i++) {
        cin >> pos1;
        add_edge(pos1, n, 1, G); 
    }

    // intersections and streets
    for (int i=0; i<m; i++) {
        cin >> pos1 >> pos2;
        add_edge(pos1, pos2, 1, G);
    }

    // edge_iterator eit1 = edges(G).first;
    // cout << "\n";
    // for (; eit1!=edges(G).second; ++eit1) {
    //     cout << boost::get(boost::edge_capacity, G, *eit1) << " " <<
    //     source(*eit1, G) << " " << target(*eit1, G) << "\n";
    // }
    // cout << "\n";

    long flow = boost::push_relabel_max_flow(G, 0, n);
    // cout << flow << "\n";

    // const auto c_map = boost::get(boost::edge_capacity, G);
    // const auto rc_map = boost::get(boost::edge_residual_capacity, G);
    // eit1 = edges(G).first;
    // cout << "\n";
    // for (; eit1!=edges(G).second; ++eit1) {
    //     cout << c_map[*eit1] - rc_map[*eit1] << " " <<
    //     source(*eit1, G) << " " << target(*eit1, G) << "\n";
    // }
    // cout << "\n";
    
    if (flow == s) cout << "yes\n";
    else cout << "no\n";

}


int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while (t--) shop();
}
