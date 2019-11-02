#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/graph/graph_utility.hpp>

typedef boost::adjacency_list_traits<boost::setS, boost::vecS, boost::directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;
typedef long capacity_t;

typedef boost::adjacency_list<
	boost::setS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<boost::edge_capacity_t, capacity_t,
		boost::property<boost::edge_residual_capacity_t, capacity_t,
			boost::property<boost::edge_reverse_t, Edge>
		>
	>> Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type CapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseMap;

void add_edge(Vertex u, Vertex v, capacity_t c, CapacityMap &cap_map, ReverseMap &rev_map, Graph &G){
	assert(u != v);
	Edge e, rev_e;
	bool s;
	boost::tie(e, s) = boost::add_edge(u, v, G);
	cap_map[e] = c;
	boost::tie(rev_e, s) = boost::add_edge(v, u, G);
	cap_map[rev_e] = 0;
	rev_map[rev_e] = e;
	rev_map[e] = rev_e;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int L, P;
		std::cin >> L >> P;
		Graph G(L + 2);
		CapacityMap cap_map = boost::get(boost::edge_capacity, G);
		ReverseMap rev_map = boost::get(boost::edge_reverse, G);

		Vertex source = L;
		Vertex sink = L+1;
		int sum = 0;
		
		for(int l=0; l<L; l++){
			int g, d;
			std::cin >> g >> d;
			add_edge(source, l, g, cap_map, rev_map, G);
			add_edge(l, sink, d, cap_map, rev_map, G);
			sum += d;
		}
		std::map<std::pair<Vertex, Vertex>, std::pair<capacity_t, capacity_t>> min_edges;

		bool feasable = true;
		for(int p=0; p<P; p++){
			int f, g, c_min, c_max;
			std::cin >> f >> g >> c_min >> c_max;
//			std::cerr << "new edge: (" << f << ", " << g << ")\n";
			if(f != g){ // reject edges with same source and target -> always satisfied
				assert(c_min <= c_max);
				std::pair<Vertex, Vertex> edge = std::make_pair(f, g);
				std::pair<Vertex, Vertex> rev_e = std::make_pair(g, f);
				// special case 1: (source, target) is already there
				if(min_edges.find(edge) != min_edges.end()){
//					std::cerr << "already there, updating\n";
					int old_c_min, old_c_max;
					std::tie(old_c_min, old_c_max) = min_edges[edge];
					min_edges[edge] = std::make_pair(old_c_min + c_min, old_c_max + c_max);
				}
				// special case 2: (target, source) is already there
				// Try without this optimisation.
				else if(false && min_edges.find(rev_e) != min_edges.end()){
//					std::cerr << "reverse already there, updating\n";
					int rev_c_min, rev_c_max;
					std::tie(rev_c_min, rev_c_max) = min_edges[rev_e];
					// check what direction to insert new node
					if(rev_c_min > c_min){
						min_edges[rev_e] = std::make_pair(rev_c_min - c_min, rev_c_max - c_min);
						min_edges[edge] = std::make_pair(0, c_max - c_min);
					}else{
						min_edges[rev_e] = std::make_pair(0, rev_c_max - rev_c_min);
						min_edges[edge] = std::make_pair(c_min - rev_c_min, c_max - rev_c_min);
					}
				}else{
					min_edges[std::make_pair(f, g)] = std::make_pair(c_min, c_max);
				}
			}
		}
		for(auto pair: min_edges){
			Vertex f, g;
			capacity_t c_min, c_max;
			std::tie(f, g) = pair.first;
			std::tie(c_min, c_max) = pair.second;
			//std::cerr << "(" << f << ", " << g << ") = (" << c_min << ", " << c_max << ")" << std::endl;
		}
		// Insert minimal edges in the graph;
		for(auto pair: min_edges){
			Vertex f, g;
			capacity_t c_min, c_max;
			std::tie(f, g) = pair.first;
			std::tie(c_min, c_max) = pair.second;
			//std::cout << c_max - c_min << "\n";
			Edge e;	
			bool s;
			boost::tie(e, s) = boost::edge(f, g, G);
			if(!s)
				add_edge(f, g, c_max - c_min, cap_map, rev_map, G);
			else{
				Vertex bridge = boost::add_vertex(G);
				add_edge(f, bridge, c_max - c_min, cap_map, rev_map, G);
				add_edge(bridge, g, c_max - c_min, cap_map, rev_map, G);
			}
			Edge g_to_sink, f_to_sink;
			if(f < L && g < L){
				boost::tie(g_to_sink, s) = boost::edge(g, sink, G);
				Edge source_to_g;
				boost::tie(source_to_g, s) = boost::edge(source, g, G);
				boost::tie(f_to_sink, s) = boost::edge(f, sink, G);
				cap_map[g_to_sink] -= c_min;
				cap_map[f_to_sink] += c_min;
			}
		}
		BGL_FORALL_EDGES(e, G, Graph){
//			std::cerr << boost::source(e, G) << "->" << boost::target(e, G) << ": " << cap_map[e] << "\n";
		}

//		std::cerr << "edges inserted" << std::endl;

		for(int l=0; l<L; l++){
			Edge e, f;
			bool s;
			boost::tie(e, s) = boost::edge(l, sink, G);
			boost::tie(f, s) = boost::edge(source, l, G);
			if(cap_map[e] < 0){
				cap_map[f] -= cap_map[e];
				sum -= cap_map[e];
				cap_map[e] = 0;
			}
		}

		if(feasable){
			int flow = boost::push_relabel_max_flow(G, source, sink);
			//std::cerr << "flow = " << flow << ", sum = " << sum << "\n";
			//std::cerr << "source = " << source << ", sink = " << sink << "\n";
			if(flow >= sum)
				std::cout << "yes\n";
			else
				std::cout << "no\n";
		}else{
			std::cout << "no\n";
		}

	}
	return 0;
}
