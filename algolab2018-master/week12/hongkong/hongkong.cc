#include <iostream>
#include <cassert>
#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::FT ET;
typedef EK::FT V;
typedef K::Point_2 P;
typedef Triangulation::Finite_faces_iterator Face_itr;
typedef Triangulation::Edge_iterator Edge_itr;
typedef Triangulation::Face_handle F_h;
typedef Triangulation::Vertex_handle V_h;

typedef unsigned __int128 uint128_t;

EK::Point_2 toEK(const K::Point_2 &P) {
    return EK::Point_2(P.x(), P.y());
}

struct Face{
	EK::FT max_radius;
	F_h face;
	const bool operator<(const Face& f2) const{
		return max_radius < f2.max_radius;
	}
};

struct Balloon{
	P location;
	double s;
};

void testcase(){
	int n, m; double r; std::cin >> n >> m >> r;
	std::vector<P> trees(n);
	std::vector<Balloon> balloons(m); 
	Triangulation tr;
	for(int i=0; i<n; i++){
		std::cin >> trees[i];
	}
	tr.insert(trees.begin(), trees.end());
	
	//precompute the max radius balloon which can take off from each face
	std::priority_queue<Face> Q;
	for(auto face_itr = tr.all_faces_begin(); face_itr != tr.all_faces_end(); face_itr++){
		Face f;
		f.face = face_itr;
		if(tr.is_infinite(face_itr)){
			f.max_radius = 1e105;
			Q.push(f);
		}else {
			EK::Point_2 p(toEK(face_itr->vertex(0)->point())), 
						q(toEK(face_itr->vertex(1)->point())), 
						r(toEK(face_itr->vertex(2)->point()));
			f.max_radius = EK::Circle_2(p, q, r).squared_radius();
			Q.push(f);
		}
	}
	std::unordered_map<F_h, EK::FT> max_takoff_radius;

	while(!Q.empty()){
		Face curr = Q.top();
		Q.pop();
		if(max_takoff_radius.count(curr.face) == 0){
			max_takoff_radius[curr.face] = curr.max_radius;
			for(int i=0; i<3; i++){
				F_h neighbor = curr.face->neighbor(i);
				if(max_takoff_radius.count(neighbor) == 0){
					EK::FT edge_squared_length = CGAL::squared_distance(
						toEK(curr.face->vertex((i+1)%3)->point()),
						toEK(curr.face->vertex((i+2)%3)->point()));
					Face n; n.face = neighbor;
					n.max_radius = CGAL::min(curr.max_radius, edge_squared_length);
					Q.push(n);
				}
			}
		}
	}

	for(int i=0; i<m; i++){
		Balloon b;
		std::cin >> b.location >> b.s;
		balloons[i] = b;
		F_h starting_face = tr.locate(b.location);
		EK::FT max_radius = max_takoff_radius[starting_face];
		V_h nearest_vertex = tr.nearest_vertex(b.location);
		V rt = r + b.s;
		V d = CGAL::squared_distance(
			toEK(nearest_vertex->point()), 
			toEK(b.location));	
		if(d < EK::FT(std::pow(b.s + r, 2))
			|| max_radius < 4*rt*rt)
			std::cout << "n";
		else
			std::cout << "y";

/*
		
		if (CGAL::has_smaller_distance_to_point(b.location, nearest_vertex->point(), K::Point_2(b.location.x()+r+b.s, b.location.y())) || max_radius < 4*rt*rt)
			std::cout << "n";
		else std::cout << "y";
*/
		

	}
	std::cout << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	for(int t=0; t<N; t++)
		testcase();
	return 0;
}
