#include <bits/stdc++.h>

bool is_valid(int i, int odd, int even){	
	return __builtin_popcount(i & odd) == __builtint_popcount(i & even);
}

int n_tiles(int i, int odd){
	return __builtint_popcount(i & odd);
}

void testcase(){

	int h, w; std::cin >> h >> w;
	std::vector<int> board(h, 0);
	// create useful masks
	int odd = 0, even = 0, m = 0;
	for(int i=0; i<w; i++){
		m |= (1 << i);
		if(i%2 == 0)
			even |= (1 << i);
		else
			odd |= (1 << i);
	}
	
	// load data
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			int e; std::cin >> e;
			board[i] |= (e << i);
		}
	}

	std::vector<std::vector<int>> mem_table(h, std::vector<int>(std::pow(2, w), 0));
	for(int i=1; i<h; i++){
		// create the bit mask
		for(int j=0; j<(1 << w); j++){
			const int s = board[i] | s;
			const int s_1 = board[i-1];
			// try to fit as many tiles as possible given the mask
			// Fit them from left to right
			for(int k=0; k<w-1; k++){
				if(board)
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
