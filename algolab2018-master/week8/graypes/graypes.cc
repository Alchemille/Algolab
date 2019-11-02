#include <iostream>
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_itr;
typedef Triangulation::Edge_iterator Edge_itr;

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	while(N > 0){
		Triangulation t;
		double min_distance = std::numeric_limits<double>::max();
		if(N > 2){
			for(int i = 0; i<N; i++){
				Triangulation::Point p;
				std::cin >> p;
				t.insert(p);
			}
			auto itr = t.finite_edges_begin();
			do{
				auto seg = t.segment(itr);
				if(seg.squared_length() < min_distance){
					min_distance = CGAL::to_double(seg.squared_length());
				}
			}while(++itr != t.finite_edges_end());
		}else{
			K::Point_2 p1; std::cin >> p1;
			K::Point_2 p2; std::cin >> p2;
			min_distance = CGAL::to_double(
				K::Segment_2(p1, p2).squared_length());
		}
		std::cout << std::ceil(sqrt(min_distance) / 2 * 100) << "\n";
		std::cin >> N;
	}
	return 0;
}
