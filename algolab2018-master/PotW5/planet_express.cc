#include <iostream>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graph_utility.hpp>

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::bidirectionalS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int>
		> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

int main(){
	std::ios_base::sync_with_stdio(false);
	int C; std::cin >> C;
	for(int c=0; c < C; c++){
		int N, M, K, T;
		std::cin >> N >> M >> K >> T;
		std::vector<int> tele_net;
		for(int t=0; t < T; t++){
			int p; std::cin >> p;
			tele_net.push_back(p);
		}
		Graph G(N);
		for(int m=0; m<M; m++){
			int u, v, w;
			std::cin >> u >> v >> w;
			boost::add_edge(u, v, w, G);
		}
		std::vector<int> components(N);
		int num = boost::strong_components(G, boost::make_iterator_property_map(components.begin(),
			boost::get(boost::vertex_index, G)));

		// Add vertices to model components
		std::vector<int> number_vertices_per_component(num, 0);
		for(int i=0; i<num; i++){
			boost::add_vertex(G);
		}
		Vertex source = N-1;
		for(int i=0; i<T; i++)
			number_vertices_per_component[components[tele_net[i]]]++;

		for(int i=0; i<T; i++){
			int c = components[tele_net[i]];
			boost::add_edge(tele_net[i], c+N, number_vertices_per_component[c]-1, G);
			boost::add_edge(c+N, tele_net[i], 0, G);
		}
		//boost::print_graph(boost::make_reverse_graph(G), boost::get(boost::vertex_index, G));
		std::vector<int> distmap(N+num);
		std::vector<int> predmap(N+num);
		boost::reverse_graph<Graph> R(G);
		
		boost::dijkstra_shortest_paths(R, source, boost::distance_map(
			boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
		
		int best_distance = std::numeric_limits<int>::max();
		for(int k=0; k<K; k++){
			int d = distmap[k];
			if(d < best_distance){
				best_distance = d;
			}
		}
		if(best_distance <= 1000000)
			std::cout << best_distance << "\n";
		else
			std::cout << "no\n";
		std::cout << std::flush;
	}
	std::cout << std::flush;
	return 0;
}
