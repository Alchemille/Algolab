#include <bits/stdc++.h>

struct Edge{
	int from, to, cost, river;
	Edge(int f, int t, int c, int r)
		: from(f), to(t), cost(c), river(r){}
};

struct Graph{
	std::vector<std::vector<Edge>> out_edges;
	Graph(int n){
		out_edges = std::vector<std::vector<Edge>>(n);
	}
	void add_edge(Edge e){
		out_edges[e.from].push_back(e);
	}
};

struct S{
	long length;
	int v;
	int k;
	S(long l, int v, int k) : length(l), v(v), k(k) {}

	const bool operator<(const S &s2) const{
		return length > s2.length;
	}
};

void testcase(){
	int n, m, k, x, y;
	std::cin >> n >> m >> k >> x >> y;
	Graph G(n);
	for(int i=0; i<m; i++){
		int f, t, c, r; std::cin >> f >> t >> c >> r;
		G.add_edge(Edge(f, t, c, r));
		G.add_edge(Edge(t, f, c, r));
	}

	// Modified Dijkstra algorithm
	std::vector<std::pair<int, int>> times_added(n, std::make_pair(0, 0));

	std::priority_queue<S> PQ;

	PQ.push(S(0l, x, 0));
	S curr = PQ.top();
	while(!PQ.empty() && !(curr.v == y && curr.k >= k)){
		curr = PQ.top();
		PQ.pop();
		for(auto e: G.out_edges[curr.v]){
			S next = S(curr.length + e.cost, e.to, curr.k + e.river);
			if((next.k > times_added[e.to].first || times_added[e.to].second == 0) && times_added[e.to].first <= k+1){
				times_added[e.to] = {next.k, times_added[e.to].second++};
				PQ.push(next);
			}
		}
	}
	std::cout << curr.length << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
