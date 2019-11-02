#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef int capacity_t;
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<boost::edge_capacity_t, capacity_t,
		boost::property<boost::edge_residual_capacity_t, capacity_t,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor>
			>
		>
	> Graph;

typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type CapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseMap;

void add_edge(Vertex u, Vertex v, capacity_t capacity, 
	CapacityMap &cap_map, ReverseMap &rev_map, Graph &G){
	assert(u != v);
	Edge e, rev_e;
	bool success;
	boost::tie(e, success) = boost::add_edge(u, v, G);
	boost::tie(rev_e, success) = boost::add_edge(v, u, G);
	cap_map[e] = capacity;
	cap_map[rev_e] = 0;
	rev_map[e] = rev_e;
	rev_map[rev_e] = e;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N, M, S;
		std::cin >> N >> M >> S;
		Graph G(N+1);
		CapacityMap cap_map = boost::get(boost::edge_capacity, G);
		ReverseMap rev_map = boost::get(boost::edge_reverse, G);

		Vertex source = 0;
		Vertex sink = N;
		for(int s=0; s<S; s++){
			int e; std::cin >> e;
			add_edge(e, sink, 1, cap_map, rev_map, G);
		}
		
		for(int m=0; m<M; m++){
			int u, v;
			std::cin >> u >> v;
			// TODO: unparallel edges
			Vertex v1 = boost::add_vertex(G);
			Vertex v2 = boost::add_vertex(G);
			add_edge(u, v1, 1, cap_map, rev_map, G);
			add_edge(v1, v, 1, cap_map, rev_map, G);
			add_edge(v1, v2, 1, cap_map, rev_map, G);
			add_edge(v, v2, 1, cap_map, rev_map, G);
			add_edge(v2, u, 1, cap_map, rev_map, G);
		}
		int flow = boost::push_relabel_max_flow(G, source, sink);
		if(flow == S)
			std::cout << "yes\n";
		else
			std::cout << "no\n";
	}
	std::cout << std::flush;
	return 0;
}
