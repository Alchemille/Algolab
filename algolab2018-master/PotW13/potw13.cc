#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
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

#define OWNER(i) (i)
#define SAILOR(j) (j+b)
#define MAX_COST 50
long max_flow(long max_matching, Vertex v_source, Vertex v_source_, Vertex v_target, Graph &G){
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	Edge edge; bool s;
	boost::tie(edge, s) = boost::edge(v_source, v_source_, G);
	assert(s); // This edge must exist
	capacitymap[edge] = max_matching;
	/*boost::tie(edge, s) = boost::edge(v_source_, v_source, G);
	assert(s);
	capacitymap[edge] = 0;
*/
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
	return s_flow * MAX_COST - cost2;
}

void testcase(){
	int b, s, p; std::cin >> b >> s >> p;
    // Create Graph and Maps
    Graph G(b+s);
	EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Add the edges
	for(int i=0; i<p; i++){
		int b_i, s_i, c_i;
		std::cin >> b_i >> s_i >> c_i;
		eaG.addEdge(OWNER(b_i), SAILOR(s_i), 1, MAX_COST - c_i);
	}
	Vertex v_source_ = boost::add_vertex(G);
	Vertex v_source = boost::add_vertex(G);
	long flow = std::min(std::min(b, s), p);
	eaG.addEdge(v_source, v_source_, flow, 0);
	Vertex v_target = boost::add_vertex(G);
	for(int i=0; i<s; i++)
		eaG.addEdge(SAILOR(i), v_target, 1, 0);
	for(int i=0; i<b; i++)
		eaG.addEdge(v_source_, OWNER(i), 1, 0);

	// Add zero edges for boat that is not chosen
	for(int i=0; i<b; i++)
		eaG.addEdge(OWNER(i), v_target, 1, MAX_COST);
/*
	do{
		std::cerr << "min = " << min_f << "; max = " << max_f << "\n";
		int mid = (max_f + min_f) / 2;
		long flow = max_flow(mid, v_source, v_source_, v_target, G);
		long flow_ = max_flow(mid+1, v_source, v_source_, v_target, G);

		if(flow >= flow_)
			max_f = mid;
		else
			min_f = mid;
	}while(min_f != max_f && max_f - min_f > 1);
	std::cout << std::max(
		max_flow(max_f, v_source, v_source_, v_target, G),
		max_flow(min_f, v_source, v_source_, v_target, G)
	) << "\n";
*/
	std::cout << max_flow(500, v_source, v_source_, v_target, G) << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int i=0; i<T; i++)
		testcase();
    return 0;
}
