#include <bits/stdc++.h>
// Code snippets demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/graph_utility.hpp>
// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w, bool min_cost=true) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
		if(min_cost){
        	boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        	capacitymap[e] = c;
		}
        weightmap[e] = w; // new!
		if(min_cost){
			capacitymap[rev_e] = 0;
			weightmap[rev_e] = -w; // new
			revedgemap[e] = rev_e; 
			revedgemap[rev_e] = e;
		}
    }
};

#define AGENT(i) (i)
#define SHELTER(i) (i+A)
// return min cost, -1 if max_flow not reached
int optimal_matching(int max_time, const int number_agents, const int number_shelters, std::vector<int> &pos_agents, std::vector<int> &pos_shelters, std::vector<std::vector<int>> &dist_maps){
	Graph G(number_agents + pos_shelters.size());
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	int A = number_agents;
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
	Vertex src = boost::add_vertex(G);
	Vertex sink = boost::add_vertex(G);

	for(int j=0; j<pos_shelters.size() ; j++){
		eaG.addEdge(SHELTER(j), sink, 1, 0);
	}

	for(int i=0; i<number_agents; i++){
		eaG.addEdge(src, AGENT(i), 1, 0);
		for(int j=0; j<number_shelters; j++){
			if(dist_maps[i][pos_shelters[j]] <= max_time){
				eaG.addEdge(AGENT(i), SHELTER(j), 1, 0); 			
			}
		}
	}
	int s_flow = boost::push_relabel_max_flow(G, src, sink);
	if(s_flow == number_agents)
		return max_time;
	else
		return -1;
}

int binary_search(int number_agents, int number_shelters, std::vector<int> &pos_agents, std::vector<int> &pos_shelters, std::vector<std::vector<int>> &dist_maps){
	int min = 0, max = 1;
	while(optimal_matching(max, number_agents, number_shelters, pos_agents, pos_shelters, dist_maps) == -1){
		min = max;
		max *= 2;	
	}
	while(min != max && max - min > 1){
		int mid = (max + min) / 2;
		int cost = optimal_matching(mid, number_agents, number_shelters, pos_agents, pos_shelters, dist_maps);
		if(cost != -1){
			max = mid;
		}else{
			min = mid;
		}
	}
	if(optimal_matching(min, number_agents, number_shelters, pos_agents, pos_shelters, dist_maps) == -1)
		min = max;
	else
		max = min;

	return max;
}

void testcase(){
	long N, M, A, S, C, D;
	std::cin >> N >> M >> A >> S >> C >> D;
    Graph G(N);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

	for(int i=0; i<M; i++){
		char w; long x, y, z;
		std::cin >> w >> x >> y >> z;
		eaG.addEdge(x, y, A, z, false);
		if(w == 'L')
			eaG.addEdge(y, x, A, z, false);
	}

	std::vector<int> pos_agents(A);
	std::vector<int> pos_shelters;
	for(int i=0; i<A; i++){
		std::cin >> pos_agents[i];
	}
	for(int i=0; i<S; i++){
		int pos; std::cin >> pos;
		pos_shelters.push_back(pos);	
		if(C == 2){
			Vertex new_shelter = boost::add_vertex(G);
			eaG.addEdge(pos, new_shelter, A, D, false);
			pos_shelters.push_back(new_shelter);
		}
	}
	std::vector<std::vector<int>> dist_maps(A, std::vector<int>(N+(C-1)*S));
	for(int i=0; i<A; i++){
		boost::dijkstra_shortest_paths(G, pos_agents[i], boost::distance_map(&dist_maps[i][0]));
	}

	int t = binary_search(pos_agents.size(), pos_shelters.size(), pos_agents, pos_shelters, dist_maps);
	std::cout << t+D << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		testcase();
	}
	return 0;
}

