#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <limits>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int>> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	int M; std::cin >> M;
	int Q; std::cin >> Q;

	for(int i=0; i<N; i++){
		int p, q;
		std::cin >> p >> q;
	}

	Graph G(N);
	for(int i=0; i<M; i++){
		int a, b, c;
		std::cin >> a >> b >> c;
		boost::add_edge(a, b, c, G);
	}
	for(int i=0; i<Q; i++){
		int src, tgt;
		std::cin >> src >> tgt;
		std::vector<int> dist_map(N);
		boost::dijkstra_shortest_paths(G, src, boost::distance_map(&dist_map[0]));
		if(dist_map[tgt] == std::numeric_limits<int>::max()){
			std::cout << "unreachable\n";
		}else{
			std::cout << dist_map[tgt] << "\n";
		}
	}
	return 0;
}
