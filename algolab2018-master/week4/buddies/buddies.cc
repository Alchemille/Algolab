// STL includes
#include <iostream>
#include <bits/stdc++.h>
#include <limits>
#include <unordered_set>
#include <set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::property<boost::edge_weight_t, int, boost::property<boost::edge_index_t, int>> EdgeProperty;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::no_property
		> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;

typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;
typedef boost::iterator_property_map<Vertex*, IndexMap, Vertex, Vertex&> MatchMap;

std::map<std::string, int> StringToId;
int current_id = 0;

int main(){
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int n, c, f;
		std::cin >> n >> c >> f;
		std::vector<std::set<int>> caracteristics(n);
		for(int i=0; i<n; i++){
			for(int j=0; j<c; j++){
				std::string s; std::cin >> s;
				if(StringToId.find(s) == StringToId.end()){
					StringToId[s] = current_id++;
				}
				caracteristics[i].insert(StringToId[s]);
			}
		}
		Graph G(n);
		
		for(int i=0; i<n; i++){
			for(int j= 0; j<n; j++){
				if(i < j){
					std::set<int> inter;
					std::set_intersection(caracteristics[i].begin(), caracteristics[i].end(), 
						caracteristics[j].begin(), caracteristics[j].end(), std::inserter(inter, inter.begin()));
					if(inter.size() > f){
						boost::add_edge(i, j, G);
					}
				}
			}
		}

		std::vector<Vertex> mates(n);
		boost::checked_edmonds_maximum_cardinality_matching(G, &mates[0]);
		bool is_a_matching = true;
		for(int i=0; i<n; i++){
		}
		BGL_FORALL_VERTICES(v, G, Graph){
			if(mates[v] == boost::graph_traits<Graph>::null_vertex()){
				is_a_matching = false;
			}
		}
		// OUTPUT
		if(is_a_matching){
			std::cout << "not optimal" << "\n";
		}else{
			std::cout << "optimal" << "\n";
		}
	}
	std::cout << std::flush;
	return 0;
}
