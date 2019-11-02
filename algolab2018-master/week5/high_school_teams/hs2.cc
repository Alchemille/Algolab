#include <bits/stdc++.h>

struct S{
	int score, players;
};

S compute_sp(std::vector<int> &skills, int s){
	int sum = 0, b = 0;
	for(int i=0; i<skills.size(); i++){
		if(s & (1 << i)){
			sum += skills[i];
			b++;
		}
	}
	return {sum, b};
}

void testcase(){
	int n, k; std::cin >> n >> k;
	std::vector<int> skills(n);
	for(int i=0; i<n; i++)
		std::cin >> skills[i];

	// split skills in two halves
	int m = skills.size()/2;
	std::vector<int> skills_a = std::vector<int>(skills.begin(), skills.begin() + m);
	std::vector<int> skills_b = std::vector<int>(skills.begin() + m, skills.end());
	std::unordered_map<int, std::vector<int>> found;

	for(int i=0; i < (1 << skills_b.size()); i++){
		S bi = compute_sp(skills_b, i);
		for(int j=i; j<(1 << skills_b.size()); j++){
			// make sure that the two sets are disjoint
			if(!(i & j)){
				S bj = compute_sp(skills_b, j);
				int diff = std::abs(bi.score - bj.score);
				int total_players = bi.players + bj.players;
				if(found.count(diff) == 0)
					found[diff] = std::vector<int>(n-k+1, 0);
				found[diff][std::min(n-k, total_players)] += ((diff != 0 || i==j) ? 1 : 2);
			}
		}
	}
	// turn the map into a cummulative map
	for(auto &v: found){
		for(int i=v.second.size()-1; i>=1; i--){
			v.second[i-1] += v.second[i];
		}
	}
	uint64_t total = 0;

	for(int i=0; i<(1<<(skills_a.size())); i++){
		S bi = compute_sp(skills_a, i);
		for(int j=i; j<(1 << (skills_a.size())); j++){
			// make sure that the two sets are disjoint
			if(!(i & j)){
				S bj = compute_sp(skills_a, j);
				int diff = std::abs(bi.score - bj.score);
				int total_players = bi.players + bj.players;
				if(found.count(diff) != 0){
					total += found[diff][n-k-std::min(total_players, n-k)]*((i==j) ? 1 : 2);
				}
			}
		}
	}

	std::cout << total << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
