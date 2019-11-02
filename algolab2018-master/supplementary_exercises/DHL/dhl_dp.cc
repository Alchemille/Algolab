#include <bits/stdc++.h>

int DP(int i, int j, std::vector<int> &p_a, std::vector<int> &p_b, std::vector<std::vector<int>> &mem_table){
	if(i == -1 && j == -1) return 0;
	if(i < 0 || j < 0 || (i == -1 && j > -1) || (i > -1 && j == -1)) return std::numeric_limits<int>::max();
	if(mem_table[i][j] != -1) return mem_table[i][j];
	
	int best_score = std::numeric_limits<int>::max();
	for(int i_ = i; i_ >= 0; i_--){
		for(int j_ = j; j_ >= 0; j_--){
			int s_round = (p_a[i+1] - p_a[i_] - (i - i_+1)) * (p_b[j+1] - p_b[j_] - (j - j_+1));
			int d = DP(i_-1, j_-1, p_a, p_b, mem_table);
			if (d != std::numeric_limits<int>::max()){
				int s = s_round + d;

				if(s < best_score)
					best_score = s;
			}
		}
	}
	mem_table[i][j] = best_score;
	return best_score;
}

void testcase(){
	int n; std::cin >> n;
	std::vector<int> a(n), b(n), p_a(n+1, 0), p_b(n+1, 0);
	for(int i=0; i<n; i++)
		std::cin >> a[i];
	for(int i=0; i<n; i++)
		std::cin >> b[i];

	p_a[1] = a[0];
	p_b[1] = b[0];
	for(int i=2; i<n+1; i++){
		p_a[i] = p_a[i-1] + a[i-1];
		p_b[i] = p_b[i-1] + b[i-1];
	}
	
	std::vector<std::vector<int>> mem_table(n, std::vector<int>(n, -1));
	std::cout << DP(n-1, n-1, p_a, p_b, mem_table) << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int i=0; i<T; i++)
		testcase();
	return 0;
}
