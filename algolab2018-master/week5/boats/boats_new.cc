#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>


int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int n; std::cin >> n;
		std::vector<std::pair<int, int>> pos_len;

		for(int i=0; i<n; i++){
			int l, p;
			std::cin >> l >> p;
			pos_len.push_back(std::make_pair(p, l));
		}

		std::sort(pos_len.begin(), pos_len.end());
		
		int n_boats = 1;
		int right_most = pos_len[0].first;
		int second_right_most = std::numeric_limits<int>::min();

		for(int i=1; i<n; i++){
			int pos = pos_len[i].first;
			int len = pos_len[i].second;

			if(right_most <= pos){
			// the boat can be fit
//				std::cout << "The boat " << i << " << can be fitted\n";
				second_right_most = right_most;
//				std::cout << "pos = " << pos << ", right_most + len = " << right_most + len << "\n";
				right_most = std::max(pos, right_most + len);
				n_boats++;
//				std::cout << "new second/right_most = " << second_right_most << " " << right_most << "\n"; 
			}else{
			// it cannot be fit. Is it worth adding it?
//				std::cout << "Conflict for boat " << i << "\n";
				if(second_right_most <= pos){
//					std::cout << "Boat " << i << "added\n";
					int potential_right_most = second_right_most + len;
					if(potential_right_most < right_most){
						right_most = potential_right_most;
					}

//				std::cout << "new second/right_most = " << second_right_most << " " << right_most << "\n"; 
				}
			}
		}

		std::cout << n_boats << std::endl;
	}

	return 0;
}
