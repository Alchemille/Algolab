#include <bits/stdc++.h>

void testcase(){
	int n; std::cin >> n;
	std::vector<int> lengths(n);
	for(int i=0; i<n; i++)
		std::cin >> lengths[i];
	std::vector<std::vector<int>> words(n);
	for(int i=0; i<n; i++){
		for(int j=0; j<lengths[i]; j++){
			int p; std::cin >> p;
			words[i].push_back(p);
		}
	}

	std::priority_queue<std::pair<int, int> , std::vector<std::pair<int, int>>,
		std::greater<std::pair<int, int>> > next_pos;
	std::deque<std::pair<int, int>> current_pos;
	
	// Init the next_pos heap
	for(int i=0; i<n; i++){
		next_pos.push(std::make_pair(words[i][0], i));	
	}
	std::vector<int> curr_indices(n, 0);
	std::vector<int> n_words(n, 0);
	int curr_distinct_words  = 0;
	bool missing = true;
	uint64_t shortest_interval = std::numeric_limits<uint64_t>::max();
	
	// Go until first solution
	std::pair<int, int> elem; 
	bool end = false;
	do{
		while(missing && !next_pos.empty()){
			elem = next_pos.top();
			next_pos.pop();
			curr_indices[elem.second]++;
			if(n_words[elem.second] == 0)
				curr_distinct_words++;
			n_words[elem.second]++;

			if(curr_indices[elem.second] < lengths[elem.second])
				next_pos.push(std::make_pair(words[elem.second][curr_indices[elem.second]], elem.second));

			if(curr_distinct_words == n)
				missing = false;

			// insert in the current heap
			current_pos.push_back(elem);
		}
		while(!missing){
			uint64_t diff = elem.first - current_pos.front().first;
			shortest_interval = std::min(shortest_interval, diff);
			std::pair<int, int> p = current_pos.front();
			current_pos.pop_front();
			n_words[p.second]--;
			if(n_words[p.second] == 0){
				missing = true;
				curr_distinct_words--;
				if(curr_indices[p.second] >= lengths[p.second]-1)
					end = true;
			}
		}
	}while(!end);
	std::cout << shortest_interval + 1 << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
