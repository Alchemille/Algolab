#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/reverse_graph.hpp>

typedef boost::adjacency_list_traits
	<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef int capacity_t;

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
typedef boost::property_map<Graph, boost::edge_reverse_t>::type RevMap;

void add_edge(Vertex u, Vertex v, capacity_t c, CapacityMap &cap_map,
	RevMap &rev_map, Graph &G){
	assert(u != v);
	Edge e, rev_e;
	bool s;
	boost::tie(e, s) = boost::add_edge(u, v, G);
	assert(s);
	cap_map[e] = c;
	boost::tie(rev_e, s) = boost::add_edge(v, u, G);
	assert(s);
	cap_map[rev_e] = 0;
	rev_map[e] = rev_e;
	rev_map[rev_e] = e;
}

#define IN_VERTEX(s) (s)
#define OUT_VERTEX(s) (s+W)

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; ++t){
		int W, N;
		std::cin >> W >> N;
		// Create datastructures
		Graph G(2*(W+1));
		CapacityMap cap_map = boost::get(boost::edge_capacity, G);
		RevMap rev_map = boost::get(boost::edge_reverse, G);
		Vertex source = 0;
		Vertex sink = W;

		// Add vertex capacity of one to avoid "clashes"
		for(int i=1; i<W; ++i){
			add_edge(IN_VERTEX(i), OUT_VERTEX(i), 1, cap_map, rev_map, G);
		}

		for(int i=0; i<N; ++i){
			int e, f;
			std::cin >> e >> f;
			//assert(e != f);
			if(e > f){
				int tmp = e;
				e = f;
				f = tmp;
			}
			//if the segment starts at the beginning, connect to source
			if(e == 0 && f != W){
				add_edge(source, IN_VERTEX(f), 1, cap_map, rev_map, G);	
			}
			// segment is at the end of a line
			if(f == W){
				//special case, no constraint on the last width
				if(e != 0)
					add_edge(OUT_VERTEX(e), sink, 1, cap_map, rev_map, G);
				else
					add_edge(source, sink, 1, cap_map, rev_map, G);
			}else{
				add_edge(OUT_VERTEX(e), IN_VERTEX(f), 1, cap_map, rev_map, G);
			}
		}
		int flow = boost::push_relabel_max_flow(G, source, sink);
		std::cout << flow << "\n";
	}	
	return 0;
}
