#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::no_property
	> Graph;
typedef Traits::vertex_descriptor Vertex;

void testcase(){
	int n, c, f; std::cin >> n >> c >> f;
	std::unordered_map<std::string, int> word_to_int;
	std::vector<std::set<int>> word_per_student(n);
	uint64_t hash = 0;
	for(int i=0; i<n; i++){
		for(int j=0; j<c; j++){
			std::string ch; std::cin >> ch;
			if(word_to_int.count(ch) == 0){
				word_to_int[ch] = hash++;	
			}
			word_per_student[i].insert(word_to_int[ch]);
		}
	}

	Graph G(n);
	
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i != j){
				std::vector<int> intersection;
				std::set_intersection(word_per_student[i].begin(), word_per_student[i].end(),
					word_per_student[j].begin(), word_per_student[j].end(), std::back_inserter(intersection));
				if(intersection.size() > f){
					boost::add_edge(i, j, G);
				}
			}
		}
	}
	std::vector<Vertex> mate(n);
	bool s = boost::checked_edmonds_maximum_cardinality_matching(G, &mate[0]);
	assert(s);
	for(auto &e: mate)
		if(e == G.null_vertex())
			s = false;
	if(s)
		std::cout << "not optimal\n";
	else
		std::cout << "optimal\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
