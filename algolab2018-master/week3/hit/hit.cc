#include <iostream>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int main(){
	std::ios_base::sync_with_stdio(false);
	int n; std::cin >> n;
	while(n > 0){
		double x; std::cin >> x;
		double y; std::cin >> y;
		double a; std::cin >> a;
		double b; std::cin >> b;

		K::Point_2 q1(x, y);
		K::Point_2 q2(a, b);

		K::Ray_2 ray(q1, q2);
		bool intersection = false;
		
		int i = 0;
		while(i < n){
			double s; std::cin >> s;
			double t; std::cin >> t;
			double u; std::cin >> u;
			double v; std::cin >> v;
			K::Point_2 p1(s, t);
			K::Point_2 p2(u, v);

			if(!intersection){
				K::Segment_2 seg(p1, p2);
				intersection = CGAL::do_intersect(seg, ray);
			}
			i++;
		}

		if(intersection){
			std::cout << "yes\n";
		}else{
			std::cout << "no\n";
		}
		std::cin >> n;
	}

	std::cout << std::flush;
	return 0;
}
