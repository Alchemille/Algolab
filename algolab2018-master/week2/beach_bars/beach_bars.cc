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
		
		int i = 0, j = 0;
		int best_number = 1;
		std::vector<std::pair<int, int>> best_ijs;
		best_ijs.push_back(std::make_pair(0, 0));
		int current_distance = 0, current_number = 1;
		bool keep_going = true;
		int max_distance = 0;

		while(keep_going){

			if(j == N-1){
				keep_going = false;
			}else{
				int following_distance = parasols_positions[j+1] - parasols_positions[j]; 
				if(current_distance + following_distance <= 201){
					j++;
					current_number++;
					current_distance += following_distance;
				}else{
					current_distance -= parasols_positions[i+1] - parasols_positions[i];
					i++;
					current_number--;
				}
				if(current_number > best_number){
					best_ijs.clear();
					best_ijs.push_back(std::make_pair(i, j));
					best_number = current_number;
					max_distance = parasols_positions[j] - parasols_positions[i];
				}else if(current_number == best_number){
					// SOLVED: The bug was that distances with differences of 1 should still map
					// to the same furtherst distance to the last parasol.
					//  i, e, dis = 199 or 200, real_dis = 99 in both cases. 
					int dis = parasols_positions[j] - parasols_positions[i];
					if(dis + (dis % 2) < max_distance){
						best_ijs.clear();
						best_ijs.push_back(std::make_pair(i, j));
						max_distance = dis + (dis % 2); 
					}else if(dis + (dis % 2) == max_distance){
						best_ijs.push_back(std::make_pair(i, j));
					}
				}
			}
		}

		if(N == 1){
			std::cout << 1 << " " << 0 << "\n";
			std::cout << parasols_positions[0] << std::endl;
		}else{
			std::cout << best_number << " " << std::ceil((double) max_distance / 2) << std::endl;
			bool first = true;
			for(auto &e: best_ijs){
				int diff = parasols_positions[e.second] - parasols_positions[e.first];
				if(!first)
					std::cout << " ";
				first = false;

				if(diff % 2 == 0){
					std::cout << parasols_positions[e.first] + diff / 2;
				}else{
					std::cout << parasols_positions[e.first] + std::floor((double) diff / 2) << " " << parasols_positions[e.first] + std::ceil((double) diff / 2);
				}
			}
		}
		std::cout << std::endl;
	}
	return 0;
}
