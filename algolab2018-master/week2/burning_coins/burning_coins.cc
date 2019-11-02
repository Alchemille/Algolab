#include <iostream>
#include <vector>

int M(int n, int i, int j, std::vector<int> &values, std::vector<int> &mem_table){
	if(i > j)
		return 0;
	if(i == j)
		return values[i];
	if(mem_table[i + j * n] != -1)
		return mem_table[i + j * n];
	
	mem_table[i + j*n] = std::max(
		values[i] + std::min(M(n, i+2, j, values, mem_table), M(n, i+1, j-1, values, mem_table)),
		values[j] + std::min(M(n, i, j-2, values, mem_table), M(n, i+1, j-1, values, mem_table))
	);

	return mem_table[i + j*n];
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int n; std::cin >> n;
		std::vector<int> values;
		for(int i=0; i<n; i++){
			int e; std::cin >> e;
			values.push_back(e);
		}
		std::vector<int> mem_table(n * n);
		std::fill(mem_table.begin(), mem_table.end(), -1);

		std::cout << M(n, 0, n-1, values, mem_table) << std::endl;

	}
	return 0;
}
