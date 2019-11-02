#include <bits/stdc++.h>

void testcase(){
	int64_t n; std::cin >> n;
	std::vector<std::pair<int64_t, int64_t>> boats(n);
	for(int i=0; i<n; i++)
		std::cin >> boats[i].second >> boats[i].first;
	std::sort(boats.begin(), boats.end());
	
	int64_t n_boats = 1;
	int64_t old_rightmost = std::numeric_limits<int64_t>::min();
	int64_t rightmost = boats[0].first;
	for(int i=1; i<n; i++){
		// test if it can still fit
//	std::cerr << "old : " << old_rightmost << "\n";
//	std::cerr << "new : " << rightmost << "\n";
		if(rightmost <= boats[i].first){
			n_boats++;
			old_rightmost = rightmost;
			rightmost = std::max(boats[i].first, rightmost + boats[i].second);
//			std::cerr << i << " " << boats[i].first << " " << boats[i].second << " can fit\n";
		}else{
			if(old_rightmost <= boats[i].first){
				int64_t new_rightmost = std::max(boats[i].first, old_rightmost + boats[i].second);
				if(new_rightmost < rightmost){
					rightmost = new_rightmost;
				}
//				std::cerr << i << " " << boats[i].first << " " << boats[i].second << " update\n";
			}
		}
	}
	std::cout << n_boats << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
