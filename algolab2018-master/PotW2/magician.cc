#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

double P(int i, int n, int k, int m, std::vector<double> &probabilities, std::vector<double> &mem_table){
    if(k <= 0){
		return 0;
    }else if(k >= m){
		return 1;
    }else if(i == n){
		return k >= m; 
    }else if(mem_table[i+k*n] != -1){
		return mem_table[i+k*n];
    }else{
		double max_p = 0;
		double q;
		for(int b = 0; b <= k; b++){
			q = probabilities[i] * P(i+1, n, k+b, m, probabilities, mem_table) + (1 - probabilities[i]) * P(i+1, n, k-b, m, probabilities, mem_table);
			if(q > max_p)
				max_p = q;
		}
		mem_table[i+k*n] = max_p;
		return max_p;
    }
}

int main(){
	std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T;
    for(int t=0; t < T; t++){
		int n; std::cin >> n;
		int k; std::cin >> k;
		int m; std::cin >> m;
		
		std::vector<double> probabilities;
		for(int i=0; i < n; i++){
			double p; std::cin >> p;
			probabilities.push_back(p);
		}

		std::vector<double> mem_table;
		for(int l=0; l<n*(m+1); l++){
			mem_table.push_back(-1.0);
		}
		std::cout << std::fixed << std::setprecision(5);
		std::cout << P(0, n, k, m, probabilities, mem_table) << std::endl;
	}
    return 0;
}
