#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

int def_rec(int i, int m, const int N, const int K, std::vector<int> &valid_starts, std::vector<int> &window_length, std::vector<std::vector<int>> &mem_table){
	//std::cout << i << " " << k << " " << m << "\n";
	if(m == 0)
		return 0;
	if(i >= N)
		return std::numeric_limits<int>::min();
	if(mem_table[i][m] != -1)
		return mem_table[i][m];

	int best_val = 0;
	if(valid_starts[i]){
		best_val = std::max(
		// Not taking the valid window
		def_rec(i+1, m, N, K, valid_starts, window_length, mem_table),
		// Take the window
		window_length[i] + def_rec(i+window_length[i], m-1, N, K, valid_starts, window_length, mem_table)
		);
	}else{
		// Not valid anyway, skip to next pos
		best_val = def_rec(i+1, m, N, K, valid_starts, window_length, mem_table);
	}
	mem_table[i][m] = best_val;
	return best_val; 		
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int n, m, k;
		std::cin >> n >> m >> k;
		std::vector<int> defense_values;
		for(int i=0; i<n; i++){
			int v; std::cin >> v;
			defense_values.push_back(v);
		}
		std::vector<int> valid_start_windows(n);
		std::vector<int> window_length(n);

		int i=0, j=-1, s=0;
		bool keep = true;
		while(keep){
			if(s < k || i == j){
				j++;
				s += defense_values[j];
			}else if(s >= k){
				s -= defense_values[i];
				i++;
			}
			if(s == k && j < n){
				valid_start_windows[i] = 1;
				window_length[i] = j - i + 1;
			}
			if(j >= n)
				keep = false;
		}
		for(auto &e: valid_start_windows)
			std::cerr << e << " ";
		std::cerr << "\n";
		// create the mem table
		std::vector<std::vector<int>> mem_table(n+1, std::vector<int>(m+1, -1));
		int res = def_rec(0, m, n, k, valid_start_windows, window_length, mem_table);
		if(res < 0)
			std::cout << "fail\n";
		else
			std::cout << res << "\n";
		std::cout << std::flush;
	}
	return 0;
}
