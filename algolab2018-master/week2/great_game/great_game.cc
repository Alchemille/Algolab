#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <climits>

int DP_shortest_path(bool maximizing, int start, int n, std::vector<int> &worst_path_from, std::vector<int> &best_path_from, std::map<int, std::set<int>> *transitions){
	if(start == n){
		return INT_MAX; 
	}
	else if(start == n-1){
		return 0;
	}
	else if(best_path_from[start] != -1 && maximizing){
		return best_path_from[start];
	}else if(worst_path_from[start] != -1 && !maximizing){
		return worst_path_from[start];
	}else if(maximizing){
		int best_path_length = INT_MAX;
		for(auto &choice: (*transitions)[start]){
			int path_length = DP_shortest_path(false, choice, n, worst_path_from, best_path_from, transitions);
			if(path_length < best_path_length)
				best_path_length = path_length;
		}
		best_path_from[start] = 1 + best_path_length;
		return best_path_from[start];
	}else{
		int worst_path_length = 0;
		for(auto &choice: (*transitions)[start]){
			int path_length = DP_shortest_path(true, choice, n, worst_path_from, best_path_from, transitions);
			if(path_length > worst_path_length)
				worst_path_length = path_length;
		}
		worst_path_from[start] = 1 + worst_path_length;
		return worst_path_from[start];
	}
}

int shortest_path(int start, int n,  std::map<int, std::set<int>> &transitions){
	const int GOAL = n-1;
	std::queue<std::pair<int, int>> D;
	D.push(std::make_pair(start, 0));

	while(!D.empty()){
		std::pair<int, int> p = D.front();
		D.pop();

		if (p.first == GOAL){
			return p.second;
		}else{
			for(auto &e: transitions[p.first]){
				D.push(std::make_pair(e, p.second+1));
			}
		}
	}
	return -1;
}
int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int n; std::cin >> n;
		int m; std::cin >> m;

		int r; std::cin >> r;
		int b; std::cin >> b;
		r--;
		b--;

		std::map<int, std::set<int>> transitions;
		
		for(int i=0; i < m; i++){
			int a; std::cin >> a;
			int b; std::cin >> b;
			a--;
			b--;
			transitions[a].insert(b);
		}
		// find shortest path for black
		std::vector<int> worst(n);
		std::fill(worst.begin(), worst.end(), -1);
		std::vector<int> best(n);
		std::fill(best.begin(), best.end(), -1);

		int shortest_b = DP_shortest_path(true, b, n, worst, best, &transitions);
		int shortest_r = DP_shortest_path(true, r, n, worst, best, &transitions);

//		std::cout << "shortest_path(r) = " << shortest_r << "\n";
//		std::cout << "shortest_path(b) = " << shortest_b << "\n";
		if(shortest_r == shortest_b){
			if(shortest_r % 2 == 0){
				std::cout << 1;
			}else{
				std::cout << 0;
			}
		}
		else if(shortest_r < shortest_b){
			std::cout << 0;
		}else{
			std::cout << 1;
		}

		std::cout << std::endl;
	}
	return 0;
}
