#include <bits/stdc++.h>

bool is_doable(const int max_boxes, std::vector<int> &strengths, std::vector<int> &weights){
	std::vector<int> assignments(strengths.size(), 0);	
	int curr_friend = 0;
	int curr_box = 0;
	while(curr_box < weights.size()){
		if(curr_friend >= strengths.size())
			return false;

		while(strengths[curr_friend] < weights[curr_box]){
			curr_friend++;
			if(curr_friend >= strengths.size())
				return false;
		}
		if(assignments[curr_friend] < max_boxes){
			curr_box++;
			assignments[curr_friend]++;
		}else{
			curr_friend++;
		}
	}
	return true;
}

void testcase(){
	int n, m; std::cin >> n >> m;
	std::vector<int> friends(n);
	std::vector<int> box_weights(m);
	for(int i=0; i<n; i++)
		std::cin >> friends[i];
	for(int i=0; i<m; i++)
		std::cin >> box_weights[i];
	
	std::sort(box_weights.begin(), box_weights.end());
	std::sort(friends.begin(), friends.end());

	// Binary search on the max number of boxes per person
	int min_n = 1;
	int max_n = m;
	if(!is_doable(m, friends, box_weights)){
		std::cout << "impossible\n";
	}else{
		while(min_n != max_n && max_n - min_n > 1){
			int mid = (max_n + min_n) / 2;
			if(is_doable(mid, friends, box_weights)){
				max_n = mid;
			}else{
				min_n = mid;
			}
		}
		if(min_n != max_n){
			if(is_doable(min_n, friends, box_weights)){
				max_n = min_n;
			}else{
				min_n = max_n;
			}
		}
		std::cout << max_n * 2 + (max_n - 1) * 1 << "\n";
	}
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int i=0; i<T; i++)
		testcase();
	return 0;
}
