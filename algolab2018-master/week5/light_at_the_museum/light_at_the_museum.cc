#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

void compute_all_possible_sums(int n, int m, std::vector<std::vector<std::pair<int, int>>> &lights, std::vector<std::vector<int>> &possible_sums, std::vector<int> &number_switches){
	for(int s = 0; s < 1 << n; s++){
		std::vector<int> sums(m, 0);
		for(int i=0; i < n; i++){
				for(int j=0; j<m; j++){
					if(s & 1 << i){
						sums[j] += lights[i][j].second;
					}else{
						sums[j] += lights[i][j].first;
					}
				}
		}
		number_switches.push_back(__builtin_popcount(s));
		possible_sums.push_back(sums);
	}
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N, M;
		std::cin >> N >> M;
		std::vector<int> target_brightness;
		for(int i=0; i<M; i++){
			int b; std::cin >> b;
			target_brightness.push_back(b);
		}

		std::vector<std::vector<std::pair<int, int>>> lights(N);
		for(int i=0; i<N; i++){
			for(int j=0; j<M; j++){
				int on, off;
				std::cin >> on >> off;
				lights[i].push_back(std::make_pair(on, off));
			}
		}

		// SPLIT AND LIST
		int split_point = N / 2;

		std::vector<std::vector<int>> possible_sums_left;
		std::vector<std::vector<int>> possible_sums_right;

		std::vector<int> number_switches_left;
		std::vector<int> number_switches_right;

		std::vector<std::vector<std::pair<int, int>>> left_lights(lights.begin(), lights.begin() + split_point);
		std::vector<std::vector<std::pair<int, int>>> right_lights(lights.begin() + split_point, lights.end());

		compute_all_possible_sums(left_lights.size(), M, left_lights, possible_sums_left, number_switches_left);
		compute_all_possible_sums(right_lights.size(), M, right_lights, possible_sums_right, number_switches_right);

		std::vector<std::pair<std::vector<int>, int>> sums_with_switches_left;
		std::vector<std::pair<std::vector<int>, int>> sums_with_switches_right;
		for(int i=0; i<possible_sums_left.size(); i++)
			sums_with_switches_left.push_back(
				std::make_pair(possible_sums_left[i], number_switches_left[i])
			);
		for(int i=0; i<possible_sums_right.size(); i++)
			sums_with_switches_right.push_back(
				std::make_pair(possible_sums_right[i], number_switches_right[i])
			);
		std::sort(sums_with_switches_left.begin(), sums_with_switches_left.end());
		std::sort(sums_with_switches_right.begin(), sums_with_switches_right.end());
		//std::sort(possible_sums_left.begin(), possible_sums_left.end());
		//std::sort(possible_sums_right.begin(), possible_sums_right.end());

		
/*		for(auto &e: sums_with_switches_left){
				std::cout << e.first[0] << " " << e.second << ", ";
		}
		std::cout << "--------------\n";
		for(auto &e: sums_with_switches_right){
				std::cout << e.first[0] << " " << e.second << ", ";
		}
		std::cout << "--------------\n";
*/		int min_lights = std::numeric_limits<int>::max();
		bool sol = false;

		// MERGE IN LINEAR TIME IN 2^(N/2)
		int i = 0, j = possible_sums_right.size()-1;

		while(i < possible_sums_left.size() && j >= 0){	
			// compute the current sum
			std::vector<int> curr_sums(M, 0);
			int n_switches;
			for(int m=0; m<M; m++){
				curr_sums[m] += sums_with_switches_left[i].first[m] + sums_with_switches_right[j].first[m];
				n_switches = sums_with_switches_left[i].second + sums_with_switches_right[j].second;
			}
/*			std::cout << "(" << i << ", " << j << "): ";
			for(auto &s: curr_sums)
				std::cout << s << ", ";
			std::cout << std::endl;
*/
			if(curr_sums == target_brightness){
				sol = true;
				if(n_switches < min_lights)
					min_lights = n_switches;
				if(j > 0 &&
					sums_with_switches_right[j-1].first == sums_with_switches_right[j].first){
					j--;
				}else if(i < possible_sums_left.size()-1 &&
					sums_with_switches_left[i+1].first == sums_with_switches_left[i].first){
					i++;
				}else if(j > 0){
					j--;
				}else{
					i++;
				}
			}else if(curr_sums < target_brightness){
				i++;
			}else{
				j--;
			}
		}
		if(sol)
			std::cout << min_lights << "\n";
		else
			std::cout << "impossible\n";
	}
	return 0;
}




/*
		std::vector<std::pair<int, int>> index_left;
		std::vector<std::pair<int, int>> index_right;

		for(int i=0; i<possible_sums_left[0].size(); i++){
			index_left.push_back(std::make_pair(possible_sums_left[0][i], i));
		}
		for(int i=0; i<possible_sums_right[0].size(); i++){
			index_right.push_back(std::make_pair(possible_sums_right[0][i], i));
		}

		std::sort(index_left.begin(), index_left.end());
		std::sort(index_right.begin(), index_right.end());
		*/


