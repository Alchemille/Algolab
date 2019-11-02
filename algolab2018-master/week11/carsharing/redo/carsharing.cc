#include <bits/stdc++.h>

struct R{
	int s, t, d, a, r;
	R(int s, int t, int d, int a, int r) : s(s), t(t), d(d), a(a), r(r) {}
	R(){}
};

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

void testcase(){
	int N, S; std::cin >> N >> S;
	std::vector<int> n_cars(N, 0);
	const int P = 100;
	const int INF = 103*S;
	for(int i=0; i<S; i++)
		std::cin >> n_cars[i];

	std::vector<std::set<int>> times(S); 	
	std::vector<R> requests(N);
	
	const int MAX_TIME = 100003;
	for(int i=0; i<S; i++){
		times[i].insert(0);
		times[i].insert(MAX_TIME);
	}

	for(int i=0; i<N; i++){
		int s, t, d, a, r; std::cin >> s >> t >> d >> a >> r;
		s--; t--;
		times[s].insert(d);
		times[t].insert(a);
		requests[i] = R(s, t, d, a, r);
	}

	std::vector<int> offset(S, 0);
	std::vector<std::unordered_map<int, int>> time_to_idx(S);
	for(int i=1; i<S; i++){
		offset[i] = offset[i-1] + times[i-1].size();
	}
	uint64_t n_nodes = 0;
	for(int i=0; i<S; i++){
		std::vector<int> t(times[i].begin(), times[i].end());
		for(int j=0; j<times[i].size(); j++){
			time_to_idx[i][t[j]] = j;
		}
		n_nodes += t.size();
	}

	// create the graph
	Graph G(n_nodes);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
	Vertex src = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);

	// insert the other edges;
	for(int i=0; i<S; i++){
		std::vector<int> t(times[i].begin(), times[i].end());
		eaG.addEdge(src, offset[i], n_cars[i], 0);

		for(int j=0; j<t.size()-1; j++){
			eaG.addEdge(j+offset[i], j+offset[i]+1, INF, P*(t[j+1]-t[j]));
		}
		// add final edge to sink
		eaG.addEdge(offset[i] + t.size()-1, target, INF, 0);
	}
	//insert the requests edges
	for(int i=0; i<N; i++){
		R r = requests[i];
		eaG.addEdge(offset[r.s] + time_to_idx[r.s][r.d], offset[r.t] + time_to_idx[r.t][r.a], 1, P*(r.a - r.d) - r.r);
	}
//	boost::print_graph(G);

	// compute min cost max flow
	boost::successive_shortest_path_nonnegative_weights(G, src, target);
    int cost = boost::find_flow_cost(G);
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
	std::cout << MAX_TIME * P * s_flow - cost << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
