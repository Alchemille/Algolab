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

void attack() {

    int n, m, u, v;
    cin >> n >> m;
    
    // build graph
    cout << "\n";
    graph G(1);
    for (int i=0; i<n; i++) {
        cin >> u >> v;
        add_edge(0, u, 1, G); // source edge

        // sequence edge (all affected segments)
        int max = (u<v)? v : m+v;
        for (int j=u+1; j<= max ; j++) {
            int a = ((j-1)%m == 0)? 10 : (j-1)%m;
            int b = (j%m == 0)? 10 : j%m;
            add_edge(a+m, b, 1, G);
            cout << max << " " << i << " " << j << " " << (j%m ) << " " << ((j-1)%m ) <<  "\n";
        }
        add_edge(v+m, 2*m+1, 1, G);
    }

    // add bottleneck vertex disjoint edges. Important to do it at the end to not have doublons
    for (int i=1; i<=m; i++) {
        add_edge(i, i+m, 1, G); // for vertex disjoint
    }

    edge_iterator eit1 = edges(G).first;
    cout << "\n";
    for (; eit1!=edges(G).second; ++eit1) {
        if (boost::get(boost::edge_capacity, G, *eit1) == 1) cout << source(*eit1, G) << " " << target(*eit1, G) << "\n";
    }
    cout << "\n";

    long flow = boost::push_relabel_max_flow(G, 0, 1+2*m);
    cout << flow << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) attack();
}
