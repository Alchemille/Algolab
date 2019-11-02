#include <bits/stdc++.h>

struct Edge{
	int from, to;
	long length;
	Edge(int from, int to, long length): from(from), to(to), length(length) {}
};

struct Vertex{
	std::vector<Edge> out_edges;
};

long testcase(){
	int n, m, k;
	int64_t x;
	std::cin >> n >> m >> x >>k;

	std::vector<Vertex> vertices(n);
	for(int i=0; i<m; i++){
		int u, v; long p;
		std::cin >> u >> v >> p;
		vertices[u].out_edges.push_back(Edge(u, v, p));
	}
	std::vector<bool> sinks(n, false);
	for(int i=0; i<n; i++){
		if(vertices[i].out_edges.empty())
			sinks[i] = true;
	}
	for(int i=0; i<n; i++){
		for(auto &e: vertices[i].out_edges){
			if(sinks[e.to])
				e.to = 0;
		}
	}
	std::vector<std::vector<int64_t>> best_scores(k+1, std::vector<int64_t>(n, -1));
	best_scores[0][0] = 0; // score for 0 steps for the root
	std::unordered_set<int> current_vertices;

	for(int i=1; i<k+1; i++){
		for(int v=0; v<n; v++){
			int64_t current_score = best_scores[i-1][v];
			if(current_score != (int64_t)-1){
				for(auto &e: vertices[v].out_edges){
					int64_t next_score = e.length + current_score;
					if(next_score > best_scores[i][e.to]){
						best_scores[i][e.to] = next_score;
					}
					// if next_score is sufficent, return it
					if(next_score >= x)
						return i;
				}
			}
		}
	}
/*	for(auto &e: best_scores){
		for(auto &f: e)
			std::cerr << f << " ";
		std::cerr << "\n";
	}
	std::cerr << x << "\n";
*/	return -1;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		long r = testcase();
		if(r > 0){
			std::cout << r << "\n";
		}else
			std::cout << "Impossible\n";
	}
	return 0;
}
