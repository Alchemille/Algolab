#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/iteration_macros.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


void testcase(){
	int h, w; std::cin >> h >> w;
	std::string note; std::cin >> note;
	int n = note.size();
	std::vector<std::vector<char>> front(h), back(h);

	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			char c; std::cin >> c;
			front[i].push_back(c);
		}
	}
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			char c; std::cin >> c;
			back[i].push_back(c);
		}
	}
	// create flow graph
	Graph G(26*26 + 26);	
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	Vertex src = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	std::vector<int> in_capacities(26*26);
	std::vector<int> out_capacities(26);

	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			char c1 = front[i][j];
			char c2 = back[i][w-j-1];
			int offset = (c1 - 'A') + (c2 - 'A')*26;
			in_capacities[offset]++;
		}
	}
	for(char &c: note){
		out_capacities[c - 'A']++;
	}
	int INF = std::numeric_limits<int>::max();

	// Add edges btw src and the tuple nodes
	for(int i=0; i<26; i++){
		for(int j=0; j<26; j++){
			eaG.addEdge(src, i+j*26, in_capacities[i+j*26]);
			eaG.addEdge(i+j*26, i+26*26, INF);
			eaG.addEdge(i+j*26, j+26*26, INF);
		}
		eaG.addEdge(i+26*26, target, out_capacities[i]);
	}
	int flow = boost::push_relabel_max_flow(G, src, target);
	if(flow == n)
		std::cout << "Yes\n";
	else
		std::cout << "No\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
