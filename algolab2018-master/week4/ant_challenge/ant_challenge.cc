// STL includes
#include <iostream>
#include <limits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>

typedef boost::property<boost::edge_weight_t, int, boost::property<boost::edge_index_t, int>> EdgeProperty;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	EdgeProperty
		> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;
typedef boost::property_map<Graph, boost::edge_index_t>::type EdgeID_Map;
typedef boost::iterator_property_map<int*, IndexMap, int, int&> DistanceMap;
typedef boost::iterator_property_map<Vertex*, IndexMap, Vertex, Vertex&> PredecessorMap;
typedef boost::iterator_property_map<int*, EdgeID_Map, int, int&> E_WeightMap;

int main(){
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int n, e, s, a, b;
		std::cin >> n >> e >> s >> a >> b;
		Graph G(n);
		std::vector<std::vector<int>> weight_maps(s, std::vector<int>(e));
		//WeightMap weight_map = boost::get(boost::edge_weight, G);

		for(int j=0; j<e; j++){
			int u, v; 
			std::cin >> u >> v;
			Edge edge; bool success;
			EdgeProperty prop;
			get_property_value(prop, boost::edge_weight) = 0;
			get_property_value(prop, boost::edge_index) = j;
			boost::tie(edge, success) = boost::add_edge(u, v, prop, G);
			for(int i=0; i<s; i++){
				int w; std::cin >> w;
				weight_maps[i][j] = w;
			}
		}

		// Compute the minimum spanning trees from the hives
		std::vector<int> hives_positions;
		IndexMap i_map = boost::get(boost::vertex_index, G);
		EdgeID_Map e_map = boost::get(boost::edge_index, G);

		std::vector<Vertex> predecessors(boost::num_vertices(G));
		WeightMap min_union_map;
		EdgeIt edge_it, edge_end;

		for(boost::tie(edge_it, edge_end) = boost::edges(G); edge_it != edge_end; edge_it++){
			min_union_map[*edge_it] = std::numeric_limits<int>::max();
		}
		for(int i=0; i<s; i++){
			// reading the hives positions. Not used for now.
			int h; std::cin >> h;
			Vertex hive = boost::vertex(h, G);
			PredecessorMap pred_map(&predecessors[0], i_map);
			E_WeightMap w_m(&(weight_maps[i])[0], e_map);

			boost::dijkstra_shortest_paths(G, hive,
			boost::weight_map(w_m).
			predecessor_map(pred_map).
			distance_combine([](int from, int to){return to;})
			);
			// Compute the union of the private networks
			BGL_FORALL_VERTICES(v, G, Graph){
				Vertex p = predecessors[v];
				Vertex q = v;
				if(p != q){
					Edge e = boost::edge(p, q, G).first;
					if(w_m[e] < min_union_map[e])
						min_union_map[e] = w_m[e];
				}
			}
		}


		// Compute the min_d{V(a), V(b)}
		Vertex source_vertex = boost::vertex(a, G);
		std::vector<int> d_map(n);

		DistanceMap distanceMap(&d_map[0], i_map);

		dijkstra_shortest_paths(G, source_vertex,
		boost::distance_map(distanceMap).
		weight_map(min_union_map));

		int min_distance = distanceMap[b];
		
		// OUTPUT
		std::cout << min_distance << "\n";
	}
	std::cout << std::flush;
	return 0;
}
