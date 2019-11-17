#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <queue>

/*
idea:
Want min cut. Edges of min-cut are limbs we pay for, from our set S to theirs T.
Want to be minimized.
Trick to find s and t: both person have to get at leat 1 figure.
Figure 0 could be in S or in T.
First build a flow with s connected only to 0. All other statues connected to t.
A second flow with s connected to all others, and 0 connected to t.
Take min of the 2 min cuts.

Again, trick to make pairs hashable for maps
*/

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > > graph;
// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

using namespace std;

void testcase() {
    int n, m;
    cin >> n >> m;
    graph G(n);

    edge_adder adder(G);

    // edges between figures aka limbs
    int a, b, c;
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        adder.add_edge(a, b, c);
    }

    int min_cut = boost::push_relabel_max_flow(G, 0, n - 1);
    int best_source = 0;
    int best_sink = n - 1;
    for (int i = 1; i < n; i++) {
        // Instead of enumerating all s-t pairs, it's enough to "rotate" the problem
        // because both you and your partner need to get at least one figure
        int source = i; // 0 --> i
        int sink = i - 1; // n - 1 ~= 0 - 1 --> i - 1
        int cut = boost::push_relabel_max_flow(G, source, sink);
        if (cut < min_cut) {
            min_cut = cut;
            best_source = source;
            best_sink = sink;
        }
    }

    // Find vertices associated to the best min cut
    // Recompute the flow to have the correct residual capacities
    min_cut = boost::push_relabel_max_flow(G, best_source, best_sink);
    auto residuals = boost::get(boost::edge_residual_capacity, G);
    vector<int> visited(n, false);
    queue<int> Q;
    visited[best_source] = true;
    int count = 1;
    Q.push(best_source);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        out_edge_it e, e_end;
        for (boost::tie(e, e_end) = boost::out_edges(u, G); e != e_end; ++e) {
            const int v = boost::target(*e, G);
            if (visited[v] || residuals[*e] == 0) {
                continue;
            }
            visited[v] = true;
            count++;
            Q.push(v);
        }
    }

    cout << min_cut << endl;

    cout << count << " ";
    for (int i = 0; i < n; ++i) {
        if (visited[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}