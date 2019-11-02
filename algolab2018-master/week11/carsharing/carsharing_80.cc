// ========
// STL includes
#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
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
        weightmap[e] = w;
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w;
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

struct Request{
	int s, t, d, a, p;
	Request(int s, int t, int d, int a, int p) : s(s), t(t), d(d), a(a), p(p) {}
	Request(){}
};

void testcase(){
	int N, S; std::cin >> N >> S;
    // Create Graph and Maps
	std::vector<int> initial_cars(S);
	for(int i=0; i<S; i++)
		std::cin >> initial_cars[i];
	std::vector<Request> requests;
	std::set<int> times;
	for(int i=0; i<N; i++){
		Request r; std::cin >> r.s >> r.t >> r.d >> r.a >> r.p; 	
		r.s--; r.t--;
		requests.push_back(r);
		times.insert(r.d);
		times.insert(r.a);
	}
	std::vector<int> times_vector = std::vector<int>(times.begin(), times.end());
    Graph G(S * times.size());
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
	
	int M = times.size();
	// Add temporal edges for cars that stay in parking lots
	for(int i=0; i<S; i++){
		for(int j=0; j<times.size()-1; j++){
			eaG.addEdge(i*M+j, i*M+j+1, std::numeric_limits<int>::max(), 100);	
		}
	}

	for(Request &r: requests){
		int d_idx = std::distance(times.begin(), times.lower_bound(r.d));
		int a_idx = std::distance(times.begin(), times.lower_bound(r.a));
		eaG.addEdge(r.s*M + d_idx, r.t*M + a_idx, 1, 100*(a_idx - d_idx) -r.p);
	}

	Vertex v_source = boost::add_vertex(G);
	Vertex v_target = boost::add_vertex(G);

	// Add source and target edges
	for(int i=0; i<S; i++){
		eaG.addEdge(v_source, i*M, initial_cars[i], 0);
		eaG.addEdge(i*M + M - 1, v_target, std::numeric_limits<int>::max(), 0);
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
	std::cout << 100 * (times.size()-1) * s_flow - cost2 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
    return 0;
}

