#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

void check_coins() {

    
    int n, m, a, b, c, score;
    cin >> n >> m;
    graph G(1); // just the source

    // complete graph

    for (int i=0; i<m; i++) {
        cin >> a >> b >> c;
        // add edges (source, tournaments)
        add_edge(0, i+1, 1, G);
        // add edges (tournament, player)
        if (c == 1 || c == 0) add_edge(i+1, 1+m+a, 1, G);
        if (c == 2 || c == 0) add_edge(i+1, 1+m+b, 1, G);
    }

    int sum_scores = 0;
    for (int i=0; i<n; i++) {
        // add edges (player, sink)
        cin >> score;
        sum_scores += score;
        add_edge(1+m+i, 1+m+n, score, G);
    }
    if (sum_scores != m) {
        cout << "no\n";
        return;
    }

    // compute max flow
    long flow = boost::push_relabel_max_flow(G, 0, 1+m+n);

    // BELOW: TO SEEE FLOW

    // const auto c_map = boost::get(boost::edge_capacity, G);
    // const auto rc_map = boost::get(boost::edge_residual_capacity, G);

    // eit1 = edges(G).first;
    // cout << "\n";
    // for (; eit1!=edges(G).second; ++eit1) {
    //     cout << c_map[*eit1] - rc_map[*eit1] << " " <<
    //     source(*eit1, G) << " " << target(*eit1, G) << "\n";
    // }
    // cout << "\n";

    //cout << flow << "\n";
    if (flow < m) cout << "no\n";
    else cout << "yes\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) check_coins();
}
