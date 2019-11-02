#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/iteration_macros.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<boost::edge_capacity_t, int,
		boost::property<boost::edge_residual_capacity_t, int, 
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor>
			>
		>
	> Graph;

typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_capacity_t>::type CapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseMap;

void add_edge(Vertex u, Vertex v, int capacity,
	CapacityMap &cap_map,
	ReverseMap &rev_map,
	Graph &G
	)
{
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
		int N, M;
		std::cin >> N >> M;
		Graph G(M+N+2);
		Vertex source = N + M;
		Vertex sink = N + M + 1;
		CapacityMap cap_map = boost::get(boost::edge_capacity, G);
		ReverseMap rev_map = boost::get(boost::edge_reverse, G);
		ResidualMap res_map = boost::get(boost::edge_residual_capacity, G);
		int sum = 0;
		
		for(int m=0; m<M; m++){
			int a, b, c;
			std::cin >> a >> b >> c;
			add_edge(source, m, 1, cap_map, rev_map, G);
			if(c == 1 || c == 0)
				add_edge(m, M+a, 1, cap_map, rev_map, G);
			if(c==2 || c == 0)
				add_edge(m, M+b, 1, cap_map, rev_map, G);
		}
		for(int n=0; n<N; n++){
			int s; std::cin >> s;
			sum += s;
			add_edge(M+n, sink, s, cap_map, rev_map, G);
		}

		int flow = boost::push_relabel_max_flow(G, source, sink);
		/*BGL_FORALL_EDGES(e, G, Graph){
			std::cout << boost::source(e, G) << "->" << boost::target(e, G) << " : " << cap_map[e] << "\n";
		}
		std::cout << "flow: " << flow << ", sum: " << sum << "\n";
		*/
		if(flow == M && flow == sum)
			std::cout << "yes\n";
		else
			std::cout << "no\n";
	}
	std::cout << std::flush;
	return 0;
}
