// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <limits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_utility.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Main
int main() {
	// build Graph
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N, M; std::cin >> N >> M;
		Graph G(N);
		EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
		ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
		ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
		EdgeAdder eaG(G, capacitymap, revedgemap);

		std::vector<int> out_cap(N, 0);
		std::vector<int> in_cap(N, 0);
		
		for(int i=0; i<M; i++){
			int f, t, c;
			std::cin >> f >> t >> c;
			eaG.addEdge(f, t, c);
			out_cap[f] += c;
			in_cap[t] += c;
		}
		int sink = N-1;
		int best_src, best_sink = N-1;
		int min_flow = std::numeric_limits<int>::max();
		for(int i=0; i<N-1; i++){
			// Find a min cut via maxflow
			int flow = boost::push_relabel_max_flow(G, i, sink);
			if(flow < min_flow){
				best_src = i;
				best_sink = sink;
				min_flow = flow;
			}
			flow = boost::push_relabel_max_flow(G, sink, i);
			if(flow < min_flow){
				best_src = sink;
				best_sink = i;
				min_flow = flow;
			}
		}
		int flow = boost::push_relabel_max_flow(G, best_src, best_sink);

		// BFS to find vertex set S
		std::vector<int> vis(N, false); // visited flags
		std::queue<int> Q; // BFS queue (from std:: not boost::)
		vis[best_src] = true; // Mark the source as visited
		Q.push(best_src);
		int nb_figures = 0;
		while (!Q.empty()) {
			const int u = Q.front();
			Q.pop();
			nb_figures++;
			OutEdgeIt ebeg, eend;
			for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
				const int v = boost::target(*ebeg, G);
				// Only follow edges with spare capacity
				if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
				vis[v] = true;
				Q.push(v);
			}
		}
		std::cout << min_flow << "\n";
		std::cout << nb_figures << " ";
		// Output S
		for (int i = 0; i < N; ++i) {
			if (vis[i]) std::cout << i << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}
