#include <iostream>
#include <vector>
#include <algorithm>

void possible_sums(std::vector<int> &list, std::vector<int> &possible_sums){
	for(int s=0; s < 1 << list.size(); s++){
		int sum = 0;
		for(int i=0; i < list.size(); i++){
			if(s & 1 << i){
				sum += list[i];
			}else{
				sum -= list[i];
			}
		}
		possible_sums.push_back(sum);
	}
}
void possible_sums_k(int k, std::vector<int> &list, std::vector<int> &possible_sums){
	for(int s=0; s < 1 << list.size(); s++){
		int sum = 0;
		for(int i=0; i < list.size(); i++){
			if(s & 1 << i){
				sum += list[i];
			}else{
				sum -= list[i];
			}
		}
		possible_sums.push_back(sum);
	}
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
		int counter = 0;
		std::vector<int> a, b;
		for(int k=0; k < 1 << N; k++){
			if(__builtin_popcount(k) <= K){
				std::vector<int> selected;
				for(int n=0; n<N; n++){
					if(~k & 1 << n){
						selected.push_back(skills[n]);
					}
				}

				// split the selected players in two teams, count if 
				// the teams have the same skill.
				const int M = selected.size();
				const int split_pt = M / 2;
				std::vector<int> list_a(selected.begin(), selected.begin() + split_pt);
				std::vector<int> list_b(selected.begin()+ split_pt, selected.end());
				
				std::vector<int> possible_sums_a, possible_sums_b;
				possible_sums(list_a, possible_sums_a);
				possible_sums(list_b, possible_sums_b);

				std::sort(possible_sums_a.begin(), possible_sums_a.end());
				std::sort(possible_sums_b.begin(), possible_sums_b.end());
				
				int cnt = 0;

				for(int i=0; i<possible_sums_a.size(); i++){
					auto itr = std::lower_bound(possible_sums_b.begin(), possible_sums_b.end(), -possible_sums_a[i]);
					if(itr != possible_sums_b.end()){
						while(*itr == -possible_sums_a[i] && itr != possible_sums_b.end()){
							cnt++;
							itr++;
						}
					}
				}
				counter += cnt;
			}
		}
		std::cout << counter << "\n";
	}
	std::cout << std::flush;
	return 0;
}

