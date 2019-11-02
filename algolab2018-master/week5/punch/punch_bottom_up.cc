#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>
#include <algorithm>
#include <cmath>

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N, K;
		std::cin >> N >> K;
		std::vector<int> prices;
		std::vector<int> volumes;
		for(int i=0; i<N; i++){
			int p, v;
			std::cin >> p >> v;
			prices.push_back(p);
			volumes.push_back(v);
		}

		int max_volume = *std::max_element(volumes.begin(), volumes.end());

		// IDEA: This almost reduces to the coin change problem.
		// Try bottom up approach this time.
		std::vector<std::vector<std::pair<int,int>>> mem_table(K+1, std::vector<std::pair<int, int>>(N, std::make_pair(-1, 0)));
		for(int v=0; v<K+1; v++){
			// fill up the table when using only one alcohol
			mem_table[v][0] = std::make_pair(std::ceil(((double)v) / volumes[0]) * prices[0], 1);
		}
		for(int i=1; i<N; i++){
			for(int v=0; v < K+1; v++){
				int price_without = mem_table[v][i-1].first;
				int drinks_without = mem_table[v][i-1].second;

				int best_price, best_drinks, price_with, drinks_with;
				if(volumes[i] >= v){
					price_with = prices[i];
					drinks_with = 1;
				}else{
					price_with = prices[i] + mem_table[v - volumes[i]][i].first;
					drinks_with = mem_table[v - volumes[i]][i].second;
					
					if(price_with == prices[i] + mem_table[v-volumes[i]][i-1].first){
						drinks_with = std::max(drinks_with, 1 + mem_table[v-volumes[i]][i-1].second);
					}
				}
				if(price_with < price_without){
					best_price = price_with;
					best_drinks = drinks_with;
				}else if(price_with == price_without){
					best_price = price_with;
					best_drinks = std::max(drinks_with, drinks_without);
				}else{
					best_price = price_without;
					best_drinks = drinks_without;
				}

				mem_table[v][i] = std::make_pair(best_price, best_drinks);
			}
		}
		std::cout << mem_table[K][N-1].first << " " << mem_table[K][N-1].second << std::endl;
		/*
		for(int i=0; i<K+1; i++)
			std::cout << i << ", ";
		std::cout << "\n";
		for(int i=0; i<N; i++){
			for(int j=0; j<K+1; j++)
				std::cout << mem_table[j][i].first << ", ";
			std::cout << "\n";
		}
		*/
	}
	std::cout << std::flush;
	return 0;
}
