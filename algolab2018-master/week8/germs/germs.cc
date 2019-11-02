#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>
#include <cassert>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

double time_from_length(double l){
	assert(l >= 0); 
    double t = sqrt(l) / 2.0 - 0.5;
	if(t < 0)
		return 0;
	double r = sqrt(t);
	assert(r>0);
	return std::ceil(r);
}

double sq_2(double i){
	assert(i >= 0);
	return i * i * 4;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	while(N>0){
		std::vector<K::Point_2> points;
		int l, b, r, t;
		std::cin >> l >> b >> r >> t;

		for(int i=0; i<N; i++){
			K::Point_2 p;
			std::cin >> p;
			points.push_back(p);
		}
		Triangulation tr;
		tr.insert(points.begin(), points.end());

		// find cause of death per bactery
		std::vector<double> min_lengths;
		for(auto v = tr.vertices_begin(); v != tr.vertices_end(); ++v){
			Triangulation::Edge_circulator c = tr.incident_edges(v);
			Triangulation::Point p = v->point();
			double min_length = std::numeric_limits<double>::max(); 
			do {
				if(c != NULL){
					if(!tr.is_infinite(c)){
						if(tr.segment(c).squared_length() < min_length){
							min_length = CGAL::to_double(tr.segment(c).squared_length());
						}
					}
				}
  			} while (c != NULL && ++c != tr.incident_edges(v));

			// Projections on the edges, take the min.
			if(sq_2(p.x() - l) <= min_length){
				min_length = sq_2(p.x() - l); 
			}
			if(sq_2(r - p.x()) <= min_length){
				min_length = sq_2(r - p.x()); 
			}
			if(sq_2(t - p.y()) <= min_length){
				min_length = sq_2(t - p.y()) ; 
			}
			if(sq_2(p.y() - b) <= min_length){
				min_length = sq_2(p.y() - b); 
			}
			assert(min_length >= 0);
			min_lengths.push_back(min_length);
		}
		// compute first death;
		double first = *std::min_element(min_lengths.begin(), min_lengths.end());
		double last = *std::max_element(min_lengths.begin(), min_lengths.end());;
		std::nth_element(min_lengths.begin(), min_lengths.begin() + min_lengths.size() / 2, min_lengths.end());
		double median = min_lengths[min_lengths.size() / 2];
		assert(first >= 0);
	//	std::cerr << first << " " << median << " " <<  last << "\n";
		std::cout << time_from_length(first) << " "
			<< time_from_length(median) << " " << time_from_length(last) <<  "\n";
		std::cin >> N;
	}
	return 0;
}
