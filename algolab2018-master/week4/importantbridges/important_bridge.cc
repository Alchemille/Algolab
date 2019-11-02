// STL includes
#include <iostream>
#include <limits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/biconnected_components.hpp>


typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_index_t, int>
		> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::property_map<Graph, boost::edge_index_t>::type EdgeID;
typedef boost::iterator_property_map<int*, EdgeID, int, int&> Component_Map;
		
int main(){
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int n, m;
		std::cin >> n >> m;
		Graph G(n);
		Graph G_copy(n);

		for(int j=0; j<m; j++){
			int u, v;
			std::cin >> u >> v;
			Edge edge; bool success;
			if(u < v){
				boost::tie(edge, success) = boost::add_edge(u, v, j, G);
				boost::tie(edge, success) = boost::add_edge(u, v, j, G_copy);
			}else{
				boost::tie(edge, success) = boost::add_edge(v, u, j, G);
				boost::tie(edge, success) = boost::add_edge(v, u, j, G_copy);
			}
		}


		std::vector<std::pair<int, int>> critical_bridges;
		std::vector<int> bicc(m);
		EdgeID e_map = boost::get(boost::edge_index, G);

		Component_Map bicc_map(&bicc[0], e_map);
		
		const int n_bcc = boost::biconnected_components(G, bicc_map);
		std::vector<int> bcc_to_number_of_edges(n_bcc);

		for(int &c: bicc){
			bcc_to_number_of_edges[c] += 1; 
		}
		BGL_FORALL_EDGES(e, G, Graph){
/*			std::cout << bcc_to_number_of_edges[bicc[e_map[e]]] << std::endl;  
			std::cout << bicc[e_map[e]] << std::endl;  
			std::cout << e_map[e] << std::endl;  
			std::cout << e << std::endl;  
*/			if(bcc_to_number_of_edges[bicc[e_map[e]]] == 1){
				Vertex first = boost::source(e, G);
				Vertex second = boost::target(e, G);
				critical_bridges.push_back(std::make_pair(first, second));
			}
		}

		std::sort(critical_bridges.begin(), critical_bridges.end());
		std::cout << critical_bridges.size() << "\n";
		for(auto &e: critical_bridges){
			std::cout << e.first << " " << e.second << "\n";
		}
	}
	std::cout << std::flush;
	return 0;
}
