#include <bits/stdc++.h>

#include <boost/pending/disjoint_sets.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef boost::disjoint_sets_with_storage<> UF;

void testcase(){
	uint64_t n, r; std::cin >> n >> r;
	std::vector<P> planets(n);
	for(int i=0; i<n; i++)
		std::cin >> planets[i];

	UF union_find(n);
	std::vector<int> set_sizes(n, 1);
	int best_size = 1;

	for(int i=n-1; i >= 0; i--){
		//check all other planet before
		for(int j=n-1; j>i; j--){
			if(CGAL::squared_distance(planets[i], planets[j]) <= K::FT(r*r)){
			//if(CGAL::has_smaller_distance_to_point(planets[i], planets[j], P(planets[i].x(), planets[i].y()+r*r))){
				int s1 = union_find.find_set(i);
				int s2 = union_find.find_set(j);
				if(s1 != s2){
					int new_size = std::min(i, set_sizes[s1] + set_sizes[s2]);
					best_size = std::max(best_size, new_size);
					// update the UF
					union_find.link(s1, s2);
					int s = union_find.find_set(s1);
					set_sizes[s] = new_size;
				}
			}
		}
	}
	std::cout << best_size << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
