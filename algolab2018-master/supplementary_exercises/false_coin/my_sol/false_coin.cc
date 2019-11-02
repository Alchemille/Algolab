#include <bits/stdc++.h>
#include <cassert>

void testcase(){
	int n, k; std::cin >> n >> k;
	std::vector<int> is_greater(n, 1);
	std::vector<int> is_less(n, 1);

	for(int i=0; i<k; i++){
		int P_i; std::cin >> P_i;
		std::vector<int> left(P_i);
		std::vector<int> right(P_i);
		std::vector<int> in_left_or_right(n, 0);
		for(int j=0; j<P_i; j++){
			std::cin >> left[j];
			in_left_or_right[left[j]-1] = 1;
		}
		for(int j=0; j<P_i; j++){
			std::cin >> right[j];
			in_left_or_right[right[j]-1] = 1;
		}
		
		char outcome; std::cin >> outcome;

		if(outcome == '<'){
			for(auto j: left)
				is_greater[j-1] = 0;
			for(auto j: right)
				is_less[j-1] = 0;
			for(int j=0; j<n; j++)
				if(!in_left_or_right[j]){
					is_greater[j] = 0;
					is_less[j] = 0;
				}
		}
		if(outcome == '>'){
			for(auto j: right)
				is_greater[j-1] = 0;
			for(auto j: left)
				is_less[j-1] = 0;
			for(int j=0; j<n; j++)
				if(!in_left_or_right[j]){
					is_greater[j] = 0;
					is_less[j] = 0;
				}
		}
		if(outcome == '='){
			for(int j=0; j<n; j++){
				if(in_left_or_right[j]){
					is_greater[j] = 0;
					is_less[j] = 0;
				}
			}
		}
	}
	std::vector<int> is_false(n, 0);
	for(int i=0; i<n; i++)
		is_false[i] += is_less[i] + is_greater[i];
	int false_coin = -1;
	bool doable = true;
	for(int i=0; i<is_false.size(); i++){
		if(is_false[i] && false_coin >= 0){
			doable = false; break;
		}
		if(is_false[i] && false_coin == -1)
			false_coin = i;
	}

	if(doable)
		std::cout << false_coin + 1 << "\n";
	else
		std::cout << 0 << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
