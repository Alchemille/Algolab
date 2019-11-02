#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>

int main(){
    int T; std::cin >> T;
    // Loop over experiments
    for(int t = 0; t < T; t++){
        int n, m, v;
        std::cin >> n;
        std::cin >> m;
        std::cin >> v;

        std::vector<std::priority_queue<int>> Q(n);

        //Insert the edges in the p_q
        for(int i = 0; i < m; i++){
            int k, l;
            std::cin >> k;
            std::cin >> l;

            Q[k].push(l);
        }
	std::stack<int> my_stack;
	my_stack.push(v);
	std::set<int> visited;
	visited.insert(v);
	while(!my_stack.empty()){
	    int current_vertex = my_stack.top();
	    my_stack.pop();
	    std::stack<int> tmp_stack;
	    while(!Q[current_vertex].empty()){
		tmp_stack.push(Q[current_vertex].top());
		Q[current_vertex].pop();
	    }
	    while(!tmp_stack.empty()){
		int elem = tmp_stack.top();
		if(visited.find(elem) != visited.end()){
		    my_stack.push(elem);
		    visited.insert(elem);
		}
		tmp_stack.pop();
	    }
	}
    }
    return 0;
}
