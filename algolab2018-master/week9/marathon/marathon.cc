#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/graph_utility.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::undirectedS> Traits;
typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;
typedef long prop_t;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, prop_t, 
		boost::property<boost::edge_capacity_t, prop_t>
	>> Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type CapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

class EdgeAdder{
	Graph &G;
	CapacityMap &cap_map;
	WeightMap &weight_map;
public:
	EdgeAdder(Graph &G, CapacityMap &cap_map, WeightMap &weight_map):
		G(G), cap_map(cap_map), weight_map(weight_map){}
	void addEdge(int u, int v, prop_t length, prop_t width){
        Edge e, rev_e;
		bool s;
		boost::tie(e, s) = boost::add_edge(u, v, G);
		cap_map[e] = width;
		weight_map[e] = length;
	}
};

//----------------------------- FLOW GRAPH DEFS ---------------------------------------------------//
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> FTraits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, FTraits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > FGraph; // new!
// Interior Property Maps
typedef boost::property_map<FGraph, boost::edge_capacity_t>::type      FEdgeCapacityMap;
typedef boost::property_map<FGraph, boost::edge_weight_t >::type       FEdgeWeightMap; // new!
typedef boost::property_map<FGraph, boost::edge_residual_capacity_t>::type FResidualCapacityMap;
typedef boost::property_map<FGraph, boost::edge_reverse_t>::type       FReverseEdgeMap;
typedef boost::graph_traits<FGraph>::vertex_descriptor          FVertex;
typedef boost::graph_traits<FGraph>::edge_descriptor            FEdge;
typedef boost::graph_traits<FGraph>::out_edge_iterator  FOutEdgeIt; // Iterator

// ===============================================================
class FEdgeAdder {
    FGraph &Gr;
    FEdgeCapacityMap &capacitymap;
    FEdgeWeightMap &weightmap;
    FReverseEdgeMap  &revedgemap;
public:
    FEdgeAdder(FGraph &G, FEdgeCapacityMap &capacitymap, FEdgeWeightMap &weightmap, FReverseEdgeMap &revedgemap) 
        : Gr(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(FVertex u, FVertex v, long c, long w) {
        FEdge e, rev_e;
		bool s;
        boost::tie(e, s) = boost::add_edge(u, v, Gr);
        boost::tie(rev_e, s) = boost::add_edge(v, u, Gr);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};
// ===============================================================

void test_case(){
	int N, M, S, F;
	std::cin >> N >> M >> S >> F;
	Graph G(N);
	WeightMap weight_map = boost::get(boost::edge_weight, G);
	CapacityMap cap_map = boost::get(boost::edge_capacity, G);
	EdgeAdder edge_adder(G, cap_map, weight_map);

	for(int i=0; i<M; i++){
		int u, v;
		long w, l;
		std::cin >> u >> v >> w >> l;
		if(u != v)
			edge_adder.addEdge(u, v, l, w);		
	}
	Vertex src = S;
	Vertex target = F;
	std::vector<long> dist_map(N);
	std::vector<long> reverse_dist_map(N);
	boost::dijkstra_shortest_paths(G, src, 
		boost::distance_map(&dist_map[0]));
	boost::dijkstra_shortest_paths(G, target, 
		boost::distance_map(&reverse_dist_map[0]));
	prop_t min_d = dist_map[target];

	// Create a new graph for the max flow
	FGraph FG(N);
    FEdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, FG);
    FEdgeWeightMap weightmap = boost::get(boost::edge_weight, FG);
    FReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, FG);
    FResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, FG);
    FEdgeAdder eaG(FG, capacitymap, weightmap, revedgemap);
	
	BGL_FORALL_EDGES(e, G, Graph){
		Vertex to = boost::target(e, G);
		Vertex from = boost::source(e, G);
		if(dist_map[from] + weight_map[e] + reverse_dist_map[to] == min_d){
			eaG.addEdge(from, to, cap_map[e], weight_map[e]);		
		}
		if(dist_map[to] + weight_map[e] + reverse_dist_map[from] == min_d){
			eaG.addEdge(to, from, cap_map[e], weight_map[e]);		
		}
	}

	long flow = boost::push_relabel_max_flow(FG, src, target);
	std::cout << flow << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		test_case();	
	}
	return 0;
}
