#include <iostream>
#include <limits>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>
#include <unordered_map>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_itr;
typedef Triangulation::Edge_iterator Edge_itr;
typedef std::unordered_map<Triangulation::Face_handle, long> EscapeMap;

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	while(N > 0){
		std::vector<K::Point_2> points(N);
		for(int i=0; i<N; i++){
			K::Point_2 p;
			std::cin >> p;
			points[i] = p;
		}
		Triangulation t;
		t.insert(points.begin(), points.end());
		std::unordered_map<Triangulation::Face_handle, long> result_map;

		for(auto face_itr = t.finite_faces_begin(); face_itr != t.finite_faces_end(); face_itr++){
			auto v0 = face_itr->vertex(0);
			auto v1 = face_itr->vertex(1);
			auto v2 = face_itr->vertex(2);

			auto s01 = K::Segment_2(v0->point(), v1->point()).squared_length();
			auto s12 = K::Segment_2(v1->point(), v2->point()).squared_length();
			auto s20 = K::Segment_2(v2->point(), v0->point()).squared_length();
			result_map[]
		}

		int M; std::cin >> M;
		for(int m=0; m<M; m++){
			bool escaped = false;
			K::Point_2 q;
			long d;
			std::cin >> q >> d;
			// Check that the person is not already violating the distance
			Triangulation::Vertex_handle v = t.nearest_vertex(q);
			if(K::Segment_2(v->point(), q).squared_length() < d){
				escaped = false;	
			}else{
				// BFS search on the faces until infinite face is reached.
				// i.e. edges of squared length >= 4 d^2
				std::queue<Triangulation::Face_handle> Q;
				Triangulation::Face_handle starting_face = t.locate(q);
				Q.push(starting_face);
				while(!Q.empty()){
					Triangulation::Face_handle current_face = Q.front();
					Q.pop();
					color_map[current_face] = SEEN;
					if(t.is_infinite(current_face)){
						escaped = true;
						for(auto itr = color_map.begin(); itr != color_map.end(); itr++){
							if(result_map.find(itr->first) == result_map.end()){
								result_map[itr->first] = d;
							}else{
								result_map[itr->first] = std::max(d, result_map[itr->first]);
							}
						}

					}else{
						// iterate of neighbor faces not already seen.
						for(int i=0; i<3; i++){
							Triangulation::Face_handle neighbor = current_face->neighbor(i);
							if(color_map.find(neighbor) == color_map.end()){
								Triangulation::Vertex_handle v1 = current_face->vertex((i + 3 -1) % 3);
								Triangulation::Vertex_handle v2 = current_face->vertex((i + 3 -2) % 3);
								// check if we can move to this face
								//std::cerr << K::Segment_2(v1->point(), v2->point()).squared_length() / 4.0 << ">=? " << d << "\n";
								if(K::Segment_2(v1->point(), v2->point()).squared_length() / 4.0 >= d){
									Q.push(neighbor);
								}
							}
						}
					}
				}
			}
			if(escaped){
				std::cout << "y";
			}else{
				std::cout << "n";
			}
		}
		std::cout << "\n";
		std::cin >> N;
	}
	return 0;
}
