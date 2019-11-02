#include <bits/stdc++.h>

typedef int Color;

long rec_fct(std::vector<int> &stacks_heights, std::vector<std::vector<Color>> &stacks, std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> &mem_table){
	// Base case := all stacks are empty
	if(stacks_heights[0] == 0 && stacks_heights[1] == 0 && stacks_heights[2] == 0 && stacks_heights[3] == 0 && stacks_heights[4] == 0)
		return 0;

	int n_stacks = stacks.size();
	int idx0 = std::max(0, stacks_heights[0]);
	int idx1 = std::max(0, stacks_heights[1]);
	int idx2 = std::max(0, stacks_heights[2]);
	int idx3 = std::max(0, stacks_heights[3]);
	int idx4 = std::max(0, stacks_heights[4]);
	if(mem_table[idx0][idx1][idx2][idx3][idx4] != -1)
		return mem_table[idx0][idx1][idx2][idx3][idx4];

	// recursion step := iterate over colors
	std::set<Color> distinct_colors;
	std::vector<Color> stack_to_color(n_stacks, -1);
	for(int i=0; i<n_stacks; i++){
		if(stacks_heights[i] > 0){
			Color c = stacks[i][stacks_heights[i]-1];
			distinct_colors.insert(c);
			stack_to_color[i] = c;
		}
	}
	int max_value = 0;
	// recurse for each color
	for(int k=1; k < (1 << n_stacks); k++){
		std::vector<int> indices;
		for(int j=0; j<n_stacks; j++){
			if(stacks_heights[j] > 0 && (k & 1 << j)){
				indices.push_back(j);
			}
		}
		// check that they have the same color
		bool recurse = true;
		if(!indices.empty()){
			Color c = stack_to_color[indices[0]];
			for(auto i: indices){
				if(stack_to_color[i] != c){
					recurse = false;	
				}
			}
		}else{
			recurse = false;
		}

		if(recurse){
			std::vector<int> stack_heights_updated(5);
			std::copy(stacks_heights.begin(), stacks_heights.end(), stack_heights_updated.begin());
			int n_color = 0;
			for(auto i: indices){
				stack_heights_updated[i]--;
				n_color++;
			}
			long score = n_color > 1 ? std::pow(2, n_color - 2) : 0;
			long res = rec_fct(stack_heights_updated, stacks, mem_table) + score;
			if(res > max_value){
				max_value = res;
			}
		}
	}
	mem_table[idx0][idx1][idx2][idx3][idx4] = max_value;
	return max_value;
}
void testcase(){
	int n; std::cin >> n;
	std::vector<int> stacks_heights(5, 0);

	for(int i=0; i<n; i++){
		std::cin >> stacks_heights[i];
	}
	std::vector<std::vector<Color>> stacks(n); 
	for(int i=0; i<n; i++)
		stacks[i] = std::vector<int>(stacks_heights[i]);

	for(int i=0; i<n; i++){
		for(int h=0; h<stacks_heights[i]; h++){
			int e; std::cin >> e;
			stacks[i][h] = e;
		}
	}

	//create memoization table
	std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> mem_table =
		std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>>(std::max(stacks_heights[0]+1, 1),
			std::vector<std::vector<std::vector<std::vector<int>>>>(std::max(stacks_heights[1]+1, 1),
				std::vector<std::vector<std::vector<int>>>(std::max(stacks_heights[2]+1, 1), 
					std::vector<std::vector<int>>(std::max(stacks_heights[3]+1, 1),
						std::vector<int>(std::max(stacks_heights[4]+1, 1), -1)))));


	long res = rec_fct(stacks_heights, stacks, mem_table);

	std::cout << res << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
