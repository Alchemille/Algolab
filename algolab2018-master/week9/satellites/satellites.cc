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
#include <cassert>
#include <algorithm>
#include <vector>
#include <queue>
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
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int g, s, l;
		std::cin >> g >> s >> l;

		Graph G(g+s);
		EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
		ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
		ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
		EdgeAdder eaG(G, capacitymap, revedgemap);

		Vertex source = boost::add_vertex(G);
		Vertex sink = boost::add_vertex(G);
			
		#define SAT(x) (x)
		#define GROUND(x) (x+s)
		
		for(int i=0; i<s; i++){
			eaG.addEdge(source, SAT(i), 1);
		}
		for(int i=0; i<g; i++){
			eaG.addEdge(GROUND(i), sink, 1);
		}
		for(int i=0; i<l; i++){
			int sat, ground;
			std::cin >> ground >> sat;
			eaG.addEdge(SAT(sat), GROUND(ground), 1);
		}

//		boost::print_graph(G);

		int N = g+s+2;
		assert(N == boost::num_vertices(G));

		// Find a min cut via maxflow
		int flow = boost::push_relabel_max_flow(G, source, sink);
//		std::cerr << "maximum flow = minimum cut = " << flow << std::endl;

		// BFS to find vertex set S
		std::vector<int> vis(N, false); // visited flags
		std::queue<int> Q; // BFS queue (from std:: not boost::)
		vis[source] = true; // Mark the source as visited
		Q.push(source);
		while (!Q.empty()) {
			const int u = Q.front();
			Q.pop();
			OutEdgeIt ebeg, eend;
			for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
				const int v = boost::target(*ebeg, G);
				// Only follow edges with spare capacity
				if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
				vis[v] = true;
				Q.push(v);
			}
		}

		// compute L and R
		std::vector<int> Sa, Gr;
		for(int i=0; i<s; i++){
			if(!vis[SAT(i)])
				Sa.push_back(i);
		}
		for(int i=0; i<g; i++)
			if(vis[GROUND(i)])
				Gr.push_back(i);

		// Output g', s'
		std::cout << Gr.size() << " " << Sa.size() << "\n";
		// Ouput indices
		for(auto &e: Gr)
			std::cout << e << " ";
		for(auto &e: Sa)
			std::cout << e << " ";
		std::cout << std::endl;
	}

	return 0;
}
