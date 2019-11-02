#include <iostream>
#include <cassert>
#include <limits>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

struct MapPart{
	std::vector<int> includes;
};
inline bool test_right_turn(P p1, P p2, P ref, P q){
	if(CGAL::right_turn(p1, p2, ref))
		std::swap(p1, p2);
	return !CGAL::right_turn(p1, p2, q);
}

void testcase(){
	int m, n; std::cin >> m >> n;
	std::vector<K::Point_2> legs;
	for(int i=0; i<m; i++){
		K::Point_2 p; std::cin >> p;
		legs.push_back(p);
	}
	std::vector<MapPart> map_parts; 	
	for(int i=0; i<n; i++){
		K::Point_2 a, b, c, d, e, f;
		std::cin >> a >> b >> c >> d >> e >> f;
		MapPart map;
		for(int j=0; j<m-1; j++){
			if(
				test_right_turn(a, b, c, legs[j]) && test_right_turn(c, d, e, legs[j]) && test_right_turn(e, f, a, legs[j]) &&
				test_right_turn(a, b, c, legs[j+1]) && test_right_turn(c, d, e, legs[j+1]) && test_right_turn(e, f, a, legs[j+1])
			){
				map.includes.push_back(j);
			}
		}
		map_parts.push_back(map);
	}
	//sliding window to find the best interval containing all legs
	int b=0, e=0;
	std::vector<int> contained(m-1, 0);
	int number_missing = m-1;
	int min_price = std::numeric_limits<int>::max();

	//std::cerr << "size(map_parts) = " << map_parts.size() << "\n";
	while(e < map_parts.size() || number_missing == 0){
		//std::cerr << "b = " << b << ", e = " << e << std::endl;
		//std::cerr << "number_missing = " << number_missing << std::endl;
		if(number_missing > 0){ // move towards right
			for(int &l: map_parts[e].includes){
				if(contained[l] == 0){
					number_missing--;
				}
				contained[l]++;
			}
			e++;
		}else{
			for(int &l: map_parts[b].includes){
				if(contained[l] == 1){
					number_missing++;
				}
				contained[l]--;
			}
			b++;
		}
		if(number_missing == 0){
			if(e - b < min_price){
				min_price = e-b;
			}
		}
	}
	std::cout << min_price << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t; t<T; t++){
		testcase();
	}
	return 0;
}
