#include <bits/stdc++.h>

uint64_t n, m, k;

struct Node{
	std::vector<int> children;
	int64_t tmp;
};

void rec_dfs(int curr, std::vector<Node> &nodes, std::vector<int> &current_line,
	std::multiset<int> &sorted_tmps, std::set<int> &results){
	Node curr_node = nodes[curr];
	current_line.push_back(curr);
	// Update the set if needed
	sorted_tmps.insert(curr_node.tmp);

	if(sorted_tmps.size() > m){
		assert(current_line.size() > m);
		auto lb = sorted_tmps.lower_bound(nodes[current_line[current_line.size()-1-m]].tmp);
		sorted_tmps.erase(lb);	
	}	

	// Check if the safe route condition is met
	if(abs(*sorted_tmps.begin() - *sorted_tmps.rbegin()) <= k && current_line.size() >= m)
		results.insert(current_line[current_line.size()-m]);	
	for(auto i: curr_node.children)
		rec_dfs(i, nodes, current_line, sorted_tmps, results);

	//maintain sorted_tmps invariant if needed
	auto lb = sorted_tmps.lower_bound(curr_node.tmp);
	sorted_tmps.erase(lb);
	if(sorted_tmps.size() < m && current_line.size() > m)
		sorted_tmps.insert(nodes[current_line[current_line.size()-1-m]].tmp);

	//maintain current_line invariant
	current_line.pop_back();
}


void testcase(){
	std::cin >> n >> m >> k;
	std::vector<Node> nodes(n);
	for(int i=0; i<n; i++)
		std::cin >> nodes[i].tmp;
	for(int i=0; i<n-1; i++){
		int u, v; std::cin >> u >> v;
		nodes[u].children.push_back(v);
	}
	std::set<int> results;
	std::multiset<int> sorted_tmps;
	std::vector<int> current_line;
	rec_dfs(0, nodes, current_line, sorted_tmps, results);
	if(results.size() > 0){
		for(auto e: results)
			std::cout << e << " ";
	}else
		std::cout << "Abort mission";
	std::cout << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
