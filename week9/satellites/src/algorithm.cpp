#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <queue>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

/*
https://www.boost.org/doc/libs/1_37_0/libs/graph/doc/adjacency_list.html
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
    int g, s, l, u, v;
    cin >> g >> s >> l;

    // build graph. 0 is source, g + s is sink
    graph G(1);
    edge_adder adder(G);

    // edges source - ground
    for (int i=0; i<g; i++) {
        adder.add_edge(0, 1 + i, 1);
    }

    // edges satellite - sink
    for (int i=0; i<s; i++) {
        adder.add_edge(1 + g + i, 1 + g + s, 1);
    }

    // edges ground - satellites
    for (int i=0; i<l; i++) {
        cin >> u >> v;
        adder.add_edge(1 + u, 1 + g + v, 1);
    }

    // compute max flow
    int flow = boost::push_relabel_max_flow(G, 0, 1 + g + s);

    // apply BFS on residual graph and record visited vertices
    vector<bool> visited(1 + g + s, false);
    queue<int> Q;
    Q.push(0);

    while (!Q.empty()) {

        const int u = Q.front();
        Q.pop();

        // push neighbors if accessible and not visited. Use iterator out_edge
        for (auto it = boost::out_edges(u, G).first; it != boost::out_edges(u, G).second; ++it) {
            // check accessible in residual graph
            int v = boost::target(*it, G);
            
            int res_cap = boost::get(boost::edge_residual_capacity, G, boost::edge(u, v, G).first);  
            if (res_cap != 0 && !visited[v]) {
                visited[v] = true; // important to say it now so that not pushed several times
                Q.push(v);
            }
        }
    }

    // count unvisited ground stations
    int ground_software = 0;
    vector<int> ground_selected;
    for (int i=1; i<1+g; i++) {
        if (!visited[i]) { 
            ground_software ++;
            ground_selected.push_back(i-1);
        }
    }

    // count visited satellites
    int satellites_software = 0;
    vector<int> satellite_selected;
    for (int i=1+g; i < 1+g+s; i++) {
        if (visited[i]) {
            satellites_software ++; 
            satellite_selected.push_back(i-1-g);
        }
    }

    cout << ground_software << " " << satellites_software << "\n";

    if (ground_selected.size() > 0 || satellite_selected.size() > 0) {
        for (auto it = ground_selected.begin(); it != ground_selected.end(); ++it) {
            cout << *it << " ";
        }
        //cout << *(ground_selected.end()) << "\n";

        for (auto it = satellite_selected.begin(); it != satellite_selected.end(); ++it) {
            cout << *it << " ";
        }
        cout << "\n";
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while(t--) testcase();
}
