#include <iostream>
#include <queue>
#include <vector>

struct Bomb{
	int timer;
	int index;
	int number_alive_parents;
	int child;

	bool operator<(const Bomb &b2) const{
		return timer > b2.timer;
	}
};

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int N; std::cin >> N;
		std::vector<Bomb> bombs(N);
		
		for(int n=0; n<N; n++){
			int v; std::cin >> v;
			bombs[n].timer = v;
			bombs[n].index = n;
			bombs[n].number_alive_parents = 0;
			bombs[n].child = -1;
		}
		for(int n=(N-1)/2-1; n >= 0; n--){
			bombs[n].number_alive_parents = 2;
		}

		for(int n=0; n<(N-1)/2; n++){
			bombs[2*n+1].timer = std::min(bombs[n].timer, bombs[2*n+1].timer);
			bombs[2*n+2].timer = std::min(bombs[n].timer, bombs[2*n+2].timer);
			bombs[2*n+1].child = n;
			bombs[2*n+2].child = n;
		}

		std::priority_queue<Bomb> upper_bombs;
		std::vector<int> seen_indices(N, 0);
		std::vector<int> finished_bombs(N, 0);
		bool success = true;
		int time = 0;
		for(int n=(N-1)/2; n<N; n++){
			upper_bombs.push(bombs[n]);
		}
		while(!upper_bombs.empty()){
			Bomb b = upper_bombs.top();
			upper_bombs.pop();
			finished_bombs[b.index] = 1;
			if(b.child != -1){
				bombs[b.child].number_alive_parents--;
				if(bombs[b.child].number_alive_parents == 0){
					upper_bombs.push(bombs[b.child]);
				}
			}
			time++;
			if(b.timer < time){
				success = false;
				break;
			}
		}
		if(success)
			std::cout << "yes\n";
		else
			std::cout << "no\n";
	}
	return 0;
}
