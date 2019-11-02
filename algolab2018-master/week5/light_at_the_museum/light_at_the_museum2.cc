#include <bits/stdc++.h>

using namespace std;

vector<vector<int64_t>> possible_sums(vector<vector<pair<int, int>>> &lights){
	vector<vector<int64_t>> sums;
	for(int s=0; s < (1 << lights.size()); s++){
		for(int j=0; j<lights[0].size(); j++){
			int64_t sum = 0;
			for(int i=0; i<lights.size(); i++){
				if(s & (1 << i)){
					sum += lights[i].first;
				}else{
					sum += lights[i].second;
				}
			}
		}
	}
}

void testcase(){
	int N, M; std::cin >> N >> M;
	std::vector<int> target(M);
	for(int i=0; i<M; i++)
		std::cin >> target[i];
	vector<vector<pair<int, int>>> lights(N);
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			int on, off; std::cin >> on >> off;
			lights[i].push_back(make_pair(on, off));
		}
	}
	int split = N/2;

}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
