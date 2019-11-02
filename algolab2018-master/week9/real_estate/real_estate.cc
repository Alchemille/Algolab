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
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
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
		int N, M, S;
		std::cin >> N >> M >> S;
		std::vector<int> nb_allowed_per_state;
		for(int i=0; i<S; i++){
			int e; std::cin >> e;
			nb_allowed_per_state.push_back(e);
		}
		Graph G(N+S+M);
		EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
		EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
		ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
		ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
		EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
		Vertex v_source = boost::add_vertex(G);
		Vertex v_target = boost::add_vertex(G);

#define STATE(x) (x)
#define REGION(x) (x+S)
#define BUYER(x) (x+S+M)
		
		// Set edges to state vertices
		for(int i=0; i<S; i++){
			eaG.addEdge(v_source, STATE(i), nb_allowed_per_state[i], 0);
		}
		// Set edges from states to regions
		for(int i=0; i<M; i++){
			int s; std::cin >> s;
			s--;
			eaG.addEdge(STATE(s), REGION(i), 1, 0);
		}
		// Set edges from regions to buyers
		for(int i=0; i<N; i++){
			for(int j=0; j<M; j++){
				int price_i_j; std::cin >> price_i_j;
				eaG.addEdge(REGION(j), BUYER(i), 1, 100-price_i_j);
			}
		}
		// Set edges from buyers to sink
		for(int i=0; i<N; i++){
			eaG.addEdge(BUYER(i), v_target, 1, 0);
		}
		// Run the algorithm
		/*int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
		boost::cycle_canceling(G);
		int cost1 = boost::find_flow_cost(G);
		std::cout << flow1 << " ";
		std::cout << -cost1 << "\n";
*/

		boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
		int cost2 = boost::find_flow_cost(G);
		int s_flow = 0;
		// Iterate over all edges leaving the source to sum up the flow values.
		OutEdgeIt e, eend;
		for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
			s_flow += capacitymap[*e] - rescapacitymap[*e];
		}
		std::cout << s_flow << " " << s_flow * 100 - cost2 << "\n";
	}

    return 0;
}
