// ALGOLAB BGL Tutorial 3
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

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N; std::cin >> N;
		Graph G(N);
		EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
		EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
		ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
		ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
		EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
		Vertex v_source = boost::add_vertex(G);
		Vertex v_target = boost::add_vertex(G);
		int MAX_PRICE = 20;
		int number_student = 0;

		for(int i=0; i<N; i++){
			int a, c;
			std::cin >> a >> c;
			eaG.addEdge(v_source, i, a, c);
		}
		for(int i=0; i<N; i++){
			int s, p;
			std::cin >> s >> p;
			number_student += s;
			eaG.addEdge(i, v_target, s, MAX_PRICE - p);
		}
		for(int i=0; i<N-1; i++){
			int v, e;
			std::cin >> v >> e;
			eaG.addEdge(i, i+1, v, e);
		}

		// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
		boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
		int cost2 = boost::find_flow_cost(G);
		int s_flow = 0;
		// Iterate over all edges leaving the source to sum up the flow values.
		OutEdgeIt e, eend;
		for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
			s_flow += capacitymap[*e] - rescapacitymap[*e];
		}
		for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
			cost2 -= (rescapacitymap[*e] - capacitymap[*e]) * MAX_PRICE;
		}
		if(s_flow >= number_student){
			std::cout << "possible ";
		}else{
			std::cout << "impossible ";
		}
		std::cout << s_flow << " " << -cost2 << "\n";
	}
    return 0;
}
