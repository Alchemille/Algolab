#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>
#include <cmath>

std::pair<int, std::unordered_set<int>> coin_change(int volume_needed, std::vector<int> &prices, std::vector<int> &volumes, std::vector<std::pair<int, std::unordered_set<int>>> &mem_table){
	if(volume_needed <= 0)
		return std::make_pair(0, std::unordered_set<int>());
	else if (mem_table[volume_needed].first != -1)
		return mem_table[volume_needed];
	else{
		int min_price = std::numeric_limits<int>::max();
		std::unordered_set<int> max_drinks;

		for(int i=0; i<prices.size(); i++){
			std::pair<int, std::unordered_set<int>> p = coin_change(
				volume_needed - volumes[i],
				prices,
				volumes,
				mem_table
			);
			if(p.first + prices[i] < min_price){
				min_price = p.first + prices[i];
				//p.second.clear();
				//p.second.insert(i);
				max_drinks = std::unordered_set<int>();
				max_drinks.insert(i);
			}else if(p.first + prices[i] == min_price){
				p.second.insert(i);
				int n_al = p.second.size();
				if(n_al > max_drinks.size()){
					max_drinks = p.second;
				}
			}
		}
		mem_table[volume_needed] = std::make_pair(min_price, max_drinks);
		return mem_table[volume_needed];
	}
}

std::pair<int, int> DP(int v, int k, std::vector<int> &prices, std::vector<int> &volumes, std::vector<std::vector<std::pair<int, int>>> &mem_table){
	if(v <= 0){
		return std::make_pair(0, 0);
	}
	if(k == 0){
		mem_table[v][k] = std::make_pair(std::ceil(((double) v) / volumes[0]) * prices[0], 1);
		return mem_table[v][k];
	}
	if(mem_table[v][k].first != -1)
		return mem_table[v][k];
	int price_with, price_without, number_drinks_with, number_drinks_without;
	std::tie(price_without, number_drinks_without) = DP(v - volumes[k], k-1, prices, volumes, mem_table);
	std::tie(price_with, number_drinks_with) = DP(v - volumes[k], k, prices, volumes, mem_table);
	if(v - volumes[k] <= 0)
		number_drinks_with++;
	if(price_without < price_with){
		mem_table[v][k] = std::make_pair(price_without + prices[k], number_drinks_without + 1);
	}else if(price_without > price_with){
		mem_table[v][k] = std::make_pair(price_with + prices[k], number_drinks_with);
	}else{
		if(number_drinks_with > number_drinks_without + 1){
			mem_table[v][k] = std::make_pair(price_with + prices[k], number_drinks_with);
		}else{
			mem_table[v][k] = std::make_pair(price_without + prices[k], number_drinks_without + 1);
		}
	}
	return mem_table[v][k];
}

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

		// IDEA: This almost reduces to the coin change problem.
		// so use DP to solve the problem
		std::vector<std::vector<std::pair<int,int>>> mem_table(K+1, std::vector<std::pair<int, int>>(N, std::make_pair(-1, 0)));
		int min_price = std::numeric_limits<int>::max(), max_drinks = 0;
		for(int k=0; k<N; k++){
			int price, drinks;
			std::tie(price, drinks) = DP(K, k, prices, volumes, mem_table);
			if(price < min_price){
				min_price = price;
				max_drinks = drinks;
			}else if(price == min_price && drinks > max_drinks){
				min_price = price;
				max_drinks = drinks;
			}
		}

		for(auto &e: mem_table){
			for(auto &f: e){
				std::cout << f.first << "|";
			}
			std::cout << "\n";
		}

		std::cout << min_price << " " << max_drinks << "\n";
	}
	std::cout << std::flush;
	return 0;
}
