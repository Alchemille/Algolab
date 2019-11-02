#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <cassert>
#include <string>
#include <unordered_map>
#include <boost/graph/graph_utility.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;

typedef int capacity_t;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::property<boost::edge_capacity_t, capacity_t, 
		boost::property<boost::edge_reverse_t, Edge,
			boost::property<boost::edge_residual_capacity_t, capacity_t>
			>
		>
	> Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type CapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseMap;

void add_edge(Vertex u, Vertex v, capacity_t c, CapacityMap &cap_map, 
	ReverseMap &rev_map, Graph &G){
	Edge e, rev_e;
	bool s;
	boost::tie(e, s) = boost::add_edge(u, v, G);
	assert(s);
	boost::tie(rev_e, s) = boost::add_edge(v, u, G);
	assert(s);

	cap_map[e] = c;
	cap_map[rev_e] = 0;
	rev_map[e] = rev_e;
	rev_map[rev_e] = e;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		Graph G(26);
		Vertex source = boost::add_vertex(G);
		Vertex sink = boost::add_vertex(G);
		CapacityMap cap_map = boost::get(boost::edge_capacity, G);
		ReverseMap rev_map = boost::get(boost::edge_reverse, G);

		int h, w; std::cin >> h >> w;
		std::string sentence; std::cin >> sentence;
		for(int i=0; i<26; i++){
			add_edge(source, i, 0, cap_map, rev_map, G);
		}
		std::vector<int> letters(26);
		for(int i=0; i<sentence.length(); i++){
			int j = (int) sentence[i] - 65;
			letters[j]++;
		}
		for(int i=0; i<26; i++){
			bool s;
			Vertex v = (int) sentence[i] - 65;
			Edge e;
			boost::tie(e, s) = boost::edge(source, i, G);
			assert(s);
			cap_map[e] += letters[i]; 
		}
		std::vector<int> front(h*w);
		std::vector<int> back(h*w);
		for(int i=0; i<h; i++){
			for(int j=0; j<w; j++){
				char c; std::cin >> c;
				int l = (int) c - 65;
				front[i*w + j] = l;
			}
		}
		for(int i=0; i<h; i++){
			for(int j=0; j<w; j++){
				char c; std::cin >> c;
				int l = (int) c - 65;
				back[i*w + j] = l;
			}
		}

		std::vector<int> middles(26*26);

		for(int i=0; i<h; i++){
			for(int j=0; j<w; j++){
				Vertex u = front[i*w + j];
				Vertex v = back[i*w + w - j - 1];
				if(v < u){
					int tmp = v;
					v = u;
					u = tmp;
				}
				Edge e; bool s;
				boost::tie(e, s) = boost::edge(u, v, G);
				if(!s){
					Vertex m = boost::add_vertex(G);
					add_edge(u, v, 1, cap_map, rev_map, G);
					add_edge(v, m, 1, cap_map, rev_map, G);
					add_edge(m, u, 1, cap_map, rev_map, G);
					add_edge(m, sink, 1, cap_map, rev_map, G);
					Edge f;
					boost::tie(f, s) = boost::edge(u, v, G);
					assert(s);
					middles[u*26+v] = m;
				}else{
					Vertex m = middles[u*26+v];
					cap_map[e] += 1;
					boost::tie(e, s) = boost::edge(v, m, G);
					assert(s);
					cap_map[e] += 1;
					boost::tie(e, s) = boost::edge(m, u, G);
					assert(s);
					cap_map[e] += 1;
					boost::tie(e, s) = boost::edge(m, sink, G);
					assert(s);
					cap_map[e] += 1;
				}
			}
		}

		capacity_t flow = boost::push_relabel_max_flow(G, source, sink);
		if(flow == sentence.length()){
			std::cout << "Yes\n";
		}else{
			std::cout << "No\n";
		}
	}
	return 0;
}
