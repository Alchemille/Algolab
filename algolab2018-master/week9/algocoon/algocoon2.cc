// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <bits/stdc++.h>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_utility.hpp>

// BGL Graph definitions
// =====================
typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int64_t>
	> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

// Main
int main() {
	// build Graph
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N, M; std::cin >> N >> M;
		Graph G(N);
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
		std::cout << std::endl;
	}
	return 0;
}
