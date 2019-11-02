#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/topological_sort.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef Traits::vertex_descriptor Vertex;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	boost::no_property,
	boost::no_property> Graph;

void testcase(){
	int n, m, k;
	std::cin >> n >> m >> k;
	std::vector<int> temperatures(n);
	Graph G(n);
	for(int i=0; i<n; i++){
		std::cin >> temperatures[i];
	}
	for(int i=0; i<n-1; i++){
		int u, v; std::cin >> u >> v;	
		boost::add_edge(u, v, G);
	}
	std::cout << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
