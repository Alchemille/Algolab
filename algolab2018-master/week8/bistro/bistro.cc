#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_itr;

int main(){
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(25);
	int N; std::cin >> N;
	while(N>0){
		Triangulation t;
		for(int i=0; i<N; i++){
			Triangulation::Point p;
			std::cin >> p;
			t.insert(p);
		}
		int M; std::cin >> M;
		for(int i=0; i<M; i++){
			Triangulation::Point p;
			std::cin >> p;
			Triangulation::Vertex_handle vertex = t.nearest_vertex(p);
			Triangulation::Point p2 = vertex->point();	
			double min_distance = K::Segment_2(p2, p).squared_length();
			std::cout << min_distance << "\n";
		}
		std::cin >> N;
	}
	return 0;
}
