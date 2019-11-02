#include <bits/stdc++.h>
int X, Y, xs, ys, xf, yf;

struct Bounds {
	int x_min, x_max, y_min, y_max;
	Bounds(int x_min, int x_max, int y_min, int y_max): 
		x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max) {}
};

struct Obstacles {
	std::vector<Bounds> bounds;
	void add_bound(int x1, int y1, int x2, int y2){
		bounds.push_back(
			Bounds(x1, x2, y1, y2)
		);
	}
	bool is_occupied(int x, int y){
		for(auto b: bounds){
			if(b.x_min <= x && x <= b.x_max
				&& b.y_min <= y && y <= b.y_max)
				return true;
		}
		return false;
	}
};

int idx_v(int vx, int vy){
	return (vx+3)*7+(vy+3);
}

long DP(int v_x, int v_y, int x, int y, Obstacles &obstacles, std::vector<std::vector<std::vector<long>>> &mem_table,
	std::vector<std::vector<std::vector<bool>>> &thrown){
	if(x == xf && y == yf) return 0;
	if(x < 0 || x >= X || y < 0 || y >= Y)
		return -1;
	if(thrown[x][y][idx_v(v_x, v_y)])
		return - 1;
	thrown[x][y][idx_v(v_x, v_y)] = true;
	if(obstacles.is_occupied(x, y)){
		return -1;
	}
	if(mem_table[x][y][idx_v(v_x, v_y)] != -2)
		return mem_table[x][y][idx_v(v_x, v_y)]; 

	long best_path = -1;

	for(int dx = -1; dx < 2; dx++){
		for(int dy = -1; dy < 2; dy++){
			int u_x = v_x + dx;
			int u_y = v_y + dy;
			if(u_x > -4 && u_x < 4
				&& u_y > -4 && u_y < 4){
				long p = DP(u_x, u_y, x+u_x, y+u_y,obstacles, mem_table, thrown);
				if(p >= 0) p++;
				if(p >= 0 && (p < best_path || best_path == -1))
					best_path = p;
			}
		}
	}
	mem_table[x][y][idx_v(v_x, v_y)] = best_path;
	return best_path;
}

void testcase(){
	std::cin >> X >> Y;
	std::cin >> xs >> ys >> xf >> yf;
	int p; std::cin >> p;
	Obstacles obstacles;

	for(int i=0; i<p; i++){
		int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
		obstacles.add_bound(x1, x2, y1, y2);
	}

	std::vector<std::vector<std::vector<long>>> mem_table(X, 
		std::vector<std::vector<long>>(Y,
			std::vector<long>(49, -2)));

	std::vector<std::vector<std::vector<bool>>> thrown(X, 
		std::vector<std::vector<bool>>(Y,
			std::vector<bool>(49, false)));

	long res = DP(0, 0, xs, ys, obstacles, mem_table, thrown);
	std::cerr << res << "\n";
	
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
