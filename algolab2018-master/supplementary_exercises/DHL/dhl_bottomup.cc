#include <bits/stdc++.h>

void testcase(){
	int n; std::cin >> n;
	std::vector<int> a(n), b(n), p_a(n+1, 0), p_b(n+1, 0);
	for(int i=0; i<n; i++)
		std::cin >> a[i];
	for(int i=0; i<n; i++)
		std::cin >> b[i];

	p_a[0] = a[0];
	p_b[0] = b[0];
	for(int i=1; i<n; i++){
		p_a[i] = p_a[i-1] + a[i-1];
		p_b[i] = p_b[i-1] + b[i-1];
	}
	
	std::vector<std::vector<int>> mem_table(n, std::vector<int>(n, -1));

	for(int i=0; i<n; i++){
		mem_table[i][0] = (p_a[i]-1-i)*(p_b[0]-1);
		mem_table[0][i] = (p_a[0]-1)*(p_b[i]-1-i);
	}

	for(int i=1; i<n; i++){
		for(int j=1; j<n; j++){
			int res = (a[i]-1)*(b[j]-1) + std::min(mem_table[i-1][j], std::min(mem_table[i][j-1], mem_table[i-1][j-1]));
			mem_table[i][j] = res;
		}
	}
	std::cout << mem_table[n-1][n-1] << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int i=0; i<T; i++)
		testcase();
	return 0;
}
