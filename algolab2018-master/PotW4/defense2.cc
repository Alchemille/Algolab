#include <bits/stdc++.h>

using namespace std;

void testcase(){
	int64_t n, m, k; std::cin >> n >> m >> k;
	std::vector<int64_t> defenses(n);
	for(int i=0; i<n; i++)
		std::cin >> defenses[i];

	// Find valid windows
	std::vector<int64_t> valid_starts(n, 0);
	std::vector<int64_t> lengths(n, 0);

	int i=0, j=-1, s=0;
	while(j < n){
		if(s < k || i == j){
			j++;
			s += defenses[j];
		}else if(s >= k){
			s -= defenses[i];
			i++;
		}
		if(s == k && j < n){
			valid_starts[i] = 1;
			lengths[i] = j-i+1;
		}
	}
	for(auto &e: valid_starts)
		cerr << e << " ";
	cerr << "\n";

	// bottom up approach this time
	std::vector<std::vector<int64_t>> mem_table(n+1, std::vector<int64_t>(m, 0));
	for(int i=0; i<m; i++)
		mem_table[n][i] = std::numeric_limits<int64_t>::min();
	for(int j=n-1; j>=0; j--){
		if(valid_starts[j]){
			mem_table[j][0] = std::max(mem_table[j+1][0], lengths[j]);
		}else{
			mem_table[j][0] = mem_table[j+1][0];
		}
	}
	
	for(int i=1; i<m; i++){
		for(int j=n-1; j>=0; j--){
			if(valid_starts[j]){
				mem_table[j][i] = std::max(mem_table[j+1][i], lengths[j] + mem_table[j+lengths[j]][i-1]);
			}else{
				mem_table[j][i] = mem_table[j+1][i];
			}
		}
	}
	for(int i=0; i<m; i++){
		for(int j=0; j<n+1; j++){
			cerr << mem_table[j][i] << " ";
		}
		cerr << "\n";
	}
	if(mem_table[0][m-1] < 0)
		cout << "fail\n";
	else
		cout << mem_table[0][m-1] << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
