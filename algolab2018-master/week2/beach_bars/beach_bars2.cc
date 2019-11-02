#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t < T; t++){
		int N; std::cin >> N;
		std::vector<int> parasols_positions;
		for(int n=0; n<N; n++){
			int p; std::cin >> p;
			parasols_positions.push_back(p);
		}

		std::sort(parasols_positions.begin(), parasols_positions.end());
		uint64_t i = 0, j = 0;
		uint64_t nb = 1, best_nb = 0, min_d = std::numeric_limits<uint64_t>::max();
		uint64_t best_i = 0, best_j = 0;
		std::vector<std::pair<int, int>> best_pos;
		while(i <= j && j<parasols_positions.size()){
			uint64_t curr_length = parasols_positions[j] - parasols_positions[i];
			if(curr_length < 201){
				if(nb > best_nb){
					best_nb = nb;
					best_i = i; best_j = j;
					best_pos.clear();
					best_pos.push_back(std::make_pair(i, j));
				}else if(nb == best_nb){
					best_pos.push_back(std::make_pair(i, j));
				}
				j++; nb++;
			}else{
				i++; nb--;
			}
		}

		// filter out bars to keep only the min distance
		std::set<int> bar_locs;
		for(auto p: best_pos){
			uint64_t diff = parasols_positions[p.second] - parasols_positions[p.first];
			uint64_t dis = 0;
			if(diff%2)
				dis = diff / 2 + 1;
			else
				dis = diff / 2;

			if(dis < min_d){
				min_d = dis;
				bar_locs.clear();
			}

			if(dis == min_d){
				if(diff%2){
					bar_locs.insert(parasols_positions[p.first]+dis);	
					bar_locs.insert(parasols_positions[p.first]+dis-1);	
				}else{
					bar_locs.insert(parasols_positions[p.first]+dis);	
				}
			}
		}
		std::cout << best_nb << " " << min_d << "\n"; 
		for(auto i: bar_locs){
			std::cout << i << " ";
		}
		std::cout << "\n";
	}
	return 0;
}
