#include <iostream>
#include <deque>
#include <vector>

int main(){
	// always put this line to speed up io ops
	std::ios_base::sync_with_stdio(false);

	int T; std::cin >> T;
	for(int t = 0; t < T; t++){
		int n; std::cin >> n;
		std::vector<int> number_occurences;

		int total = 0;
		for(int i = 0; i < n; i++){
			int e; std::cin >> e;
			number_occurences.push_back(e);
			total += e;
		}

		std::vector<std::deque<int>> words_positions;

		for(int i = 0; i < n; i++){
			int J = number_occurences[i];
			std::deque<int> ith_positions;
			for(int j = 0; j < J; j++){
				int e; std::cin >> e;
				ith_positions.push_back(e);
			}
			words_positions.push_back(ith_positions);
		}
		std::vector<std::pair<int, int>> merged_list;
		merged_list.reserve(total);

		// Compute the merged list in O(n) time
		bool keep_going = true;
		while(keep_going){
			// find next word to insert in the list
			int min_index = -1;
			int word = -1;
			for(int m = 0; m < n; m++){
				if(!words_positions[m].empty()){
					int elem; elem = words_positions[m].front();
					if(min_index == -1 || elem < min_index){
						min_index = elem;
						word = m;
					}
				}
			}
			if(word != -1){
				// insert this word in the list
				merged_list.push_back(std::make_pair(word, min_index));
				// delete the word from the list
				words_positions[word].pop_front();
			}else{
				keep_going = false;
			}
		}
		
		// Then sliding window algo
		std::vector<int> included(n);
		int i = 0;
		int j = 0;
		int best_i, best_j;
		included[merged_list[0].first] = 1;
		int best_size = -1;
		int m = 1;
		keep_going = true;
		while(keep_going){
			if(m == n && (best_size == -1 || (merged_list[j].second - merged_list[i].second + 1) < best_size)){
				best_size = merged_list[j].second - merged_list[i].second + 1;
				best_i = i;
				best_j = j;
			}
			if(m < n && j == merged_list.size()-1)
				keep_going = false;
			else if(m < n){
				j++;
				if(included[merged_list[j].first] == 0)
					m++;
				included[merged_list[j].first] += 1;
			}else{
				included[merged_list[i].first] -= 1;
				if(included[merged_list[i].first] == 0)
					m--;
				i++;
			}
		}
		std::cout << best_size << "\n";
	}	
	return 0;
}
