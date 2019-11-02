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
	ResidualCapacityMap &rescapacitymap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap, ResidualCapacityMap &rescapacitymap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap), rescapacitymap(rescapacitymap) {}

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

	std::pair<int, int> flow_for(int i, Graph &G, Vertex src, Vertex sink, Vertex a, Vertex k){
		boost::remove_edge(src, k, G);
		boost::remove_edge(a, sink, G);
		addEdge(src, k, i, 0);
		addEdge(a, sink, i, 0);
		// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
		boost::successive_shortest_path_nonnegative_weights(G, src, sink);
		int cost2 = boost::find_flow_cost(G);
		int s_flow = 0;
		// Iterate over all edges leaving the source to sum up the flow values.
		OutEdgeIt e, eend;
		for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
			s_flow += capacitymap[*e] - rescapacitymap[*e];
		}
		return std::make_pair(s_flow, cost2);
	}
};

void testset(){
	int c, g, b, k, a;
	std::cin >> c >>g >> b >> k >> a;
	Graph G(c);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap, rescapacitymap);
	int max_trunks = 0;
	for(int i=0; i<g; i++){
		int x, y, d, e;
		std::cin >> x >> y >> d >> e;
		eaG.addEdge(x, y, e, d);	
	}
	Vertex src = boost::add_vertex(G);
	Vertex sink = boost::add_vertex(G);
	eaG.addEdge(src, k, 0, 0);
	eaG.addEdge(a, sink, 0, 0);
	int s_flow = 1;
	int cost;
	int min_trunks = 0;
	bool keep = true;
	// binary search on the number of trunks
	for(int i = 1; keep; i *= 2){
		boost::tie(s_flow, cost) = eaG.flow_for(i, G, src, sink, a, k);
		if(s_flow == i && cost <= b){
			max_trunks = i*2;
			min_trunks = i;
		}else{
			keep = false;
		}
	}


	// Perform binary search;
	int flow;
	while(min_trunks != max_trunks && min_trunks + 1 != max_trunks){
		int mid = min_trunks + (max_trunks - min_trunks) / 2;
		boost::tie(flow, cost) = eaG.flow_for(mid, G, src, sink, a, k);
		if(flow == mid && cost <= b){
			min_trunks = mid;
		}else{
			max_trunks = mid;
		}
	}
	if(min_trunks != max_trunks){
		boost::tie(flow, cost) = eaG.flow_for(max_trunks, G, src, sink, a, k);
		if(flow == max_trunks && cost <= b){
			min_trunks = max_trunks;
		}
	}

	std::cout << min_trunks << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testset();
	return 0;
}
