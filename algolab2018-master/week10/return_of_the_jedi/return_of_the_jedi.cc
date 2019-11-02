#include <bits/stdc++.h>

#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/iteration_macros.hpp>


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::undirectedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;

typedef long prop_t;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<
		boost::edge_weight_t, prop_t>
	> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

long MST_cost(std::vector<Edge> MST, WeightMap &wm){
	long cost = 0;
	for(auto e: MST)
		cost += wm[e];
	return cost;
}

void testcase(){
	int n; int i;
	std::cin >> n >> i;
	Graph G(n);
	WeightMap weight_map = boost::get(boost::edge_weight, G);
	std::vector<std::vector<long>> weights = std::vector<std::vector<long>>(n, std::vector<long>(n));
	for(int j=0; j<n-1; j++){
		for(int k=1; k<n-j; k++){
			int cost; std::cin >> cost;
			boost::add_edge(j, j+k, cost, G);
			weights[j][j+k] = cost;
			weights[j+k][j] = cost;
			//std::cerr << "added edges btw " << j << " and " << j+k << "\n";
		}
	}
	std::vector<Edge> MST;
	kruskal_minimum_spanning_tree(G, std::back_inserter(MST));
	prop_t min_weight = std::numeric_limits<prop_t>::max();

	Graph MST_G(n);
	for(auto edge : MST){
		boost::add_edge(
		boost::source(edge, G),
		boost::target(edge, G),
		weight_map[edge],
		MST_G);
	}

	//construct a max(node_i, node_j) table in n²
	std::vector<std::vector<long>> dists(n, std::vector<long>(n, -1));

	BGL_FORALL_VERTICES(v, MST_G, Graph){
		// Depth first search from this node to annotate all distances
		// -> can be done as by def, there is only one path between too
		// nodes in the MST
		std::stack<Vertex> Q;
		std::stack<long> maxes;
		Q.push(v);
		maxes.push(0);

		while(!Q.empty()){
			Vertex curr = Q.top();
			long max = maxes.top();
			Q.pop(); maxes.pop();

			BGL_FORALL_OUTEDGES(curr, e, MST_G, Graph){
				Vertex dest = boost::target(e, MST_G);
				if(dists[v][dest] == -1){
					long max_d = std::max(max, weights[curr][dest]);
					dists[v][dest] = max_d;
					maxes.push(max_d);
					Q.push(dest);
				}
			}
		}
	}
/*
	for(auto v: dists){
		for(auto d: v)
			std::cerr << d << ", ";
		std::cerr << "\n";
	}
*/	
	long best_diff = std::numeric_limits<long>::max();

	for(Vertex from = 0; from < n-1; from++){
		for(Vertex to = from+1; to < n; to++){
			/*Vertex from = boost::source(e, G);
			Vertex to = boost::target(e, G);*/
			bool s; Edge e;
			boost::tie(e, s) = boost::edge(from, to, MST_G);
			if(s) continue;

			best_diff = std::min(weights[to][from] - dists[to][from], best_diff);
		}
	}
	std::cout << MST_cost(MST, weight_map) + best_diff << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		testcase();
	}
}
