#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;
typedef std::pair<P, long> Contestant;

int sum_(std::vector<bool> v){
	int s=0;
	for(auto e: v)
		s+=e;
	return s;
}

std::vector<int> survivors(const int h, const int N, const int M, int lower, int upper, const std::vector<P> &lamps, std::vector<Contestant> &contestants, std::vector<bool> &eliminated){
	Triangulation tr;
	tr.insert(lamps.begin()+lower, lamps.begin() + upper);
	std::vector<int> surv;

	// Perform the upper - lower queries to know who remains
	for(int i=0; i<M; i++){
		Triangulation::Vertex_handle nearest_lamp = tr.nearest_vertex(contestants[i].first);
		if(CGAL::squared_distance(nearest_lamp->point(), contestants[i].first) >= std::pow(h + contestants[i].second, 2)){
			surv.push_back(i);
		}else{
			eliminated[i] = true;
		}
	}
	return surv;
}

void testcase(){
	int M, N; std::cin >> M >> N;
	std::vector<std::pair<P, long>> contestants(M);

	for(int i=0; i<M; i++){
		std::cin >> contestants[i].first >> contestants[i].second;	 	
	}
	int h; std::cin >> h;
	std::vector<P> lamps(N);
	for(int i=0; i<N; i++){
		std::cin >> lamps[i];
	}
	
	int upper = N;
	int n_eliminated = 0;
	std::vector<bool> eliminated(M, false);
	std::vector<bool> dummy(M, false);
	
	std::vector<int> res = survivors(h, N, M, 0, upper, lamps, contestants, dummy);
	if(res.empty()){
		// Binary search	
		int lower = 0;
		int upper = N;
		while(upper - lower > 1){
			int m = (lower+upper)/2;
			std::vector<bool> elim_copy(M, false);
			std::copy(eliminated.begin(), eliminated.end(), elim_copy.begin());

			res = survivors(h, N, M, lower, m, lamps, contestants, elim_copy);
//			std::cerr << lower << " " << upper << " " << sum_(elim_copy) << " " << contestants.size() << "\n";

			if(sum_(elim_copy) == contestants.size()){
				upper = m;
			}else{
				std::copy(elim_copy.begin(), elim_copy.end(), eliminated.begin());
				lower = m;
			}

		}
//		std::cerr << "n = " << N << "\n";
//		std::cerr << "idx = " << upper << "\n";
		res = survivors(h, N, M, 0, upper-1, lamps, contestants, dummy);
	}
	for(auto &i: res)
		std::cout << i << " ";
	std::cout << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}

