// STL includes
#include <iostream>
#include <limits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int>
	> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;
typedef boost::iterator_property_map<int*, IndexMap, int, int&> DistanceMap;

int main(){
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int n; std::cin >> n;
		int m; std::cin >> m;

		Graph G(n);
		WeightMap weight_map = boost::get(boost::edge_weight, G);
		for(int i=0; i<m; i++){
			int start, end, weight;
			bool success;
			Edge e;
			std::cin >> start >> end >> weight; boost::tie(e, success) = boost::add_edge(start, end, G);
			weight_map[e] = weight;
		}

		//compute the minimum spanning tree
		std::vector<Edge> min_tree;
		boost::kruskal_minimum_spanning_tree(G, std::back_inserter(min_tree));
		
		unsigned long sum_of_w = 0;
		for(auto &e: min_tree){
			sum_of_w += weight_map[e];
		}
		
		// Compute the max_i{distance(Node(0), Node(i))}
		Vertex source_vertex = boost::vertex(0, G);
		std::vector<int> d_map(n);
		IndexMap i_map = boost::get(boost::vertex_index, G);

		DistanceMap distanceMap(&d_map[0], i_map);


		dijkstra_shortest_paths(G, source_vertex,
		boost::distance_map(distanceMap));
		
		int max_length = 0;
		for(auto &d: d_map){
			if(d > max_length)
				max_length = d;
		}
		// OUTPUT
		std::cout << sum_of_w << " " << max_length << "\n";
	}
	std::cout << std::flush;
	return 0;
}
