#include <bits/stdc++.h>

void testcase(){
	int n; std::cin >> n;
	std::vector<int> heights(5, 0);
	for(int i=0; i<n; i++)
		std::cin >> heights[i];

	std::vector<std::vector<int>> stacks(n);
	for(int i=0; i<n; i++){
		for(int j=0; j<heights[i]; j++){
			int c; std::cin >> c;
			stacks[i].push_back(c);
		}
	}

	std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> mem_table(heights[0]+1,
		std::vector<std::vector<std::vector<std::vector<int>>>>(std::max(1, heights[1]+1), 
			std::vector<std::vector<std::vector<int>>>(std::max(1, heights[2]+1), 
				std::vector<std::vector<int>>(std::max(1, heights[3]+1), 
					std::vector<int>(std::max(1, heights[4]+1), -1)))));
	
	// bottom up approach
	for(int i=0; i<heights[0]; i++){
		for(int j=0; j<heights[1]; j++){
			for(int k=0; k<heights[2]; k++){
				for(int l=0; l<heights[3]; l++){
					for(int m=0; m<heights[4]; m++){
						std::set<int> distinct_colors;
						if(i != 0) distinct_colors.insert(stacks[0][i]);
						if(j != 0) distinct_colors.insert(stacks[1][j]);
						if(k != 0) distinct_colors.insert(stacks[2][k]);
						if(l != 0) distinct_colors.insert(stacks[3][l]);
						if(m != 0) distinct_colors.insert(stacks[4][m]);

						for(auto c : distinct_colors){
							
						}
					}
				}
			}
		}
	}
	
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
