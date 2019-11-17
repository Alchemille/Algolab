#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <unordered_map>
#include <boost/functional/hash.hpp>

/*
First idea:
Want min cut. Edges of min-cut are limbs we pay for, from our set S to theirs T.
Want to be minimized.
Trick to find s and t: both person have to get at leat 1 figure.
Figure 0 could be in S or in T. 
First build a flow with s connected only to 0. All other statues connected to t.
A second flow with s connected to all others, and 0 connected to t.
Take min of the 2 min cuts.
Again, trick to make pairs hashable for maps

Problem : 
Because all non 0 figures go to sink with infinite edge,
the cut was necessarily S = 0, T = [1:n-1] in the first graph and
S = [1:n-1], T=0 in the second graph

Solution: 
s and t should be connected to just 1 node. Instead of trying all pairs (would be n^2 maxflow calls),
can rotate nodes (i, i+1) with s=i, t=i+1. No need to change the graph, just change 2 edges, for s and t.
Reason works: 
For every combination C of 1, 2, ... , n-1 figures, there exists a pair (u, u+1) such that
u in C and u+1 is not, or u+1 in C and u is not.

!! residual_capacity of e in boost is what can still be fill for this edge, not its reverse !!
No need for a master s and t, can directly use the figures. 
*/

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;
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
    for (int i=0; i<m; i++) {
        cin >> a >> b >> c;
        adder.add_edge(a, b, c);
    }

    // fist attempt: s fist figure, t second figure
    int best_source = n-1;
    int best_sink = 0;
    long min_cut = boost::push_relabel_max_flow(G, best_source, best_sink);

    // rotate s and t to find the min cut
    for (int i=0; i<n-1; i++) {
        int source = i;
        int sink = i+1;
        if (boost::push_relabel_max_flow(G, source, sink) < min_cut) {
            min_cut = boost::push_relabel_max_flow(G, source, sink);
            best_source = source;
            best_sink = sink;
        }
    }

    // once best sink and best source found, find S and T with BFS
    min_cut = boost::push_relabel_max_flow(G, best_source, best_sink);
    
    int count = 1;
    queue<int> Q;
    vector<bool> visited(n, false);
    Q.push(best_source);
    visited[best_source] = true;

    while(!Q.empty()) {
        int figure = Q.front();
        Q.pop();

        for (auto it = boost::out_edges(figure, G).first; it!=boost::out_edges(figure, G).second; ++it) {
            int v = boost::target(*it, G);
            if (!visited[v] && boost::get(boost::edge_residual_capacity, G, *it) != 0) { //  capacity edge is not full
                count ++;
                visited[v] = true;
                Q.push(v);
            }
        }
    }

    cout << min_cut << "\n";
    cout << count << " ";
    for (int i=0; i<n; i++) {
        if (visited[i]) cout << i << " ";
    }
    cout  << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
