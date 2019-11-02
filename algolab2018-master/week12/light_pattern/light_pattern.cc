#include <bits/stdc++.h>
void print_lights(int k, std::vector<int> lights){
	for(int i: lights){
		for(int j=0; j<k; j++){
			if(i & (1 << j))
				std::cerr << "1";
			else
				std::cerr << "0";
		}
		std::cerr << " ";
	}
	std::cerr << "\n";
}
void testcase(){
	int n, k, x; std::cin >> n >> k >> x;
	std::vector<int> lights;
	int mask = 0;
	for(int i=0; i<k-1; i++)
		mask = (mask | 1) << 1;
	mask |= 1;

	for(int i = 0; i<n/k; i++){
		int e = 0;
		for(int j=0; j<k; j++){
			int b; std::cin >> b;
			if(b)
				e = e | 1;
			if(j < k-1)
				e = e << 1;
		}
		lights.push_back(e);
	}
	std::vector<std::vector<int>> mem_table(n/k, std::vector<int>(2, -1));
	// init the mem_table
	mem_table[0][0] = std::min(__builtin_popcount(lights[0] ^ x), 1 + __builtin_popcount((~lights[0] & mask) ^ x));
	mem_table[0][1] = std::min(1 + __builtin_popcount(lights[0] ^ x), __builtin_popcount((~lights[0] & mask) ^ x));

	for(int i=1; i<n/k; i++){
		for(int j=0; j<2; j++){
			mem_table[i][0] = std::min(__builtin_popcount(lights[i] ^ x) + mem_table[i-1][0], 1 + __builtin_popcount((~lights[i] & mask) ^ x) + mem_table[i-1][1]);
			mem_table[i][1] = std::min(1 + __builtin_popcount(lights[i] ^ x) + mem_table[i-1][0], __builtin_popcount((~lights[i] & mask) ^ x) + mem_table[i-1][1]);
		}
	}

	std::cout << mem_table[n/k-1][0] << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
