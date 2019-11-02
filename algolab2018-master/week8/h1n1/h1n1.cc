#include <cassert>
#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_itr;
typedef Triangulation::Edge_iterator Edge_itr;
typedef std::set<Triangulation::Face_handle> ColorSet;
typedef Triangulation::Face_handle F_h;
typedef Triangulation::Vertex_handle V_h;
typedef K::FT ET;

struct Face_info{
	ET squared_radius;
	F_h face;
	const bool operator<(const Face_info &f2) const{
		return squared_radius < f2.squared_radius;
	}
};

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
		// Pre compute the max escape squared radius per face
		std::unordered_map<F_h, ET> max_squared_radii;
		std::priority_queue<Face_info> Q;
		for(auto f_itr = t.faces_begin(); f_itr != t.faces_end(); f_itr++){
			for(int i=0; i<3; i++){
				F_h neighbor = f_itr->neighbor(i);
				if(t.is_infinite(neighbor) && max_squared_radii.count(neighbor) == 0){
					Face_info f;
					f.face = neighbor;
					f.squared_radius = ET(std::numeric_limits<double>::max());
					Q.push(f);
				}
			}
		}

		while(!Q.empty()){
			Face_info f = Q.top();
			Q.pop();
			if(max_squared_radii.count(f.face) == 0){
				max_squared_radii[f.face] = f.squared_radius;
				for(int i=0; i<3; i++){
					F_h neighbor = f.face->neighbor(i);
					ET squared_edge = CGAL::squared_distance(
						f.face->vertex((i+1)%3)->point(),
						f.face->vertex((i+2)%3)->point()
					);
					Face_info n; n.face = neighbor;
					n.squared_radius = CGAL::min(squared_edge, f.squared_radius); 
					Q.push(n);
				}
			}
		};

		int M; std::cin >> M;
		for(int m=0; m<M; m++){
			K::Point_2 location; std::cin >> location;
			ET radius; std::cin >> radius;
			V_h nearest_vertex = t.nearest_vertex(location);
			if(CGAL::squared_distance(nearest_vertex->point(), location) < radius){
				std::cout << "n";
			}else{
				// find the included face
				F_h starting_face = t.locate(location);
				//std::cerr << "max allowed radius = " << max_squared_radii[starting_face] << "\n";
				//std::cerr << "person radius = " << radius << "\n";
				if(max_squared_radii[starting_face] < 4*radius){
					std::cout << "n";
				}else{
					std::cout << "y";
				}
			}
		}
		std::cout << "\n";
		std::cin >> N;
	}
	return 0;
}
