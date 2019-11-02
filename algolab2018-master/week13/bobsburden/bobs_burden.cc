#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<
		boost::edge_weight_t, long
	>> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

void testcase(){
	int k; std::cin >> k;
	Graph G(k*(k+1)/2);
	std::vector<int> node_values(k*(k+1)/2);
	int node_idx = 0;
	for(int d=1; d<k+1; d++){
		for(int i=0; i<d; i++){
			int val; std::cin >> val;
			node_values[node_idx] = val;
			if(d < k){
				boost::add_edge(node_idx, (d)*(d+1)/2+i, val, G);
				boost::add_edge(node_idx, (d)*(d+1)/2+i+1, val, G);
			}
			if(i < d-1){
				if(d > 1){
					boost::add_edge(node_idx, node_idx+1, val, G);
					boost::add_edge(node_idx, (d-2)*(d-1)/2+i, val, G);
				}
			}
			if(i > 0){
				if(d > 1){
					boost::add_edge(node_idx, node_idx-1, val, G);
					boost::add_edge(node_idx, (d-2)*(d-1)/2+i-1, val, G);
				}
			}
			node_idx++;
		}
	}
	// perform the three dijkstras
	std::vector<long> from_v1(node_idx), from_v2(node_idx), from_v3(node_idx);
	boost::dijkstra_shortest_paths(G, 0, boost::distance_map(&from_v1[0]));
	boost::dijkstra_shortest_paths(G, (k-1)*k/2, boost::distance_map(&from_v2[0]));
	boost::dijkstra_shortest_paths(G, k*(k+1)/2 - 1, boost::distance_map(&from_v3[0]));
	
	long best_distance = std::numeric_limits<long>::max();

	BGL_FORALL_VERTICES(v, G, Graph){
		long path_length = 0;
		path_length += from_v1[v];
		path_length += from_v2[v];
		path_length += from_v3[v];
		path_length += node_values[v];

		if(path_length < best_distance)
			best_distance = path_length;
	}
	std::cout << best_distance << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
