#include <bits/stdc++.h>

std::vector<std::pair<int, int>> find_possible_sums(int k, int i, std::vector<int> &list, std::vector<std::pair<int, int>> &possible_sums){
	if(i == list.size()) return possible_sums;
	std::vector<std::pair<int, int>> elem_to_add;
	if(i == 0)
		possible_sums.push_back(
			std::make_pair(0, 0)
		);
	if(i < list.size()){
		for(auto &t: possible_sums){
			if(t.second + 1 <= k){
				elem_to_add.push_back(
					std::make_pair(t.first, t.second+1));
			}
			elem_to_add.push_back(
				std::make_pair(t.first + list[i], t.second));
			elem_to_add.push_back(
				std::make_pair(t.first - list[i], t.second));
		}
	}

	return	find_possible_sums(k, i+1, list, elem_to_add);
}

int main(){
	std::ios_base::sync_with_stdio(false);

	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N, K;
		std::cin >> N >> K;
		std::vector<int> skills;
		for(int n=0; n<N; n++){
			int s; std::cin >> s;
			skills.push_back(s);
		}
		int split_point = N/2;
		std::vector<int> list_a(skills.begin(), skills.begin() + split_point);
		std::vector<int> list_b(skills.begin() + split_point, skills.end());
		
		//find possible sums for both
		std::vector<std::pair<int, int>> possible_sums_a, possible_sums_b;
		possible_sums_a = find_possible_sums(K, 0, list_a, possible_sums_a);
		possible_sums_b = find_possible_sums(K, 0, list_b, possible_sums_b);

		std::sort(possible_sums_a.begin(), possible_sums_a.end());
		std::sort(possible_sums_b.begin(), possible_sums_b.end());
/*
		for(auto p:possible_sums_a)
			std::cerr << p.first << " " << p.second << "\n";
*/
		int cnt = 0;
		for(int i=0; i<possible_sums_a.size(); i++){
			int s_a = possible_sums_a[i].first;
			int k_a = possible_sums_a[i].second;

			auto itr = std::lower_bound(possible_sums_b.begin(), possible_sums_b.end(), std::make_pair(-s_a, 0));
			while(itr != possible_sums_b.end() && (*itr).first == -s_a){
				if((*itr).second + k_a <= K)
					cnt++;
				itr++;
			}
		}

		std::cout << cnt << "\n";
	}
	std::cout << std::flush;
	return 0;
}
