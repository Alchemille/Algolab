#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef K::Point_2 P;
typedef IK::Point_2 IP;
typedef K::Line_2 L;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
typedef IK::Segment_2 IS;


double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(0);
	int n; std::cin >> n;
	while(n > 0){
		double x; std::cin >> x;
		double y; std::cin >> y;
		double a; std::cin >> a;
		double b; std::cin >> b;

		IK::Point_2 q1(x, y);
		IK::Point_2 q2(a, b);
		K::Point_2 e1(x, y), e2(a, b);
		K::Ray_2 ray(e1, e2);
		IK::Ray_2 i_ray(q1, q2);

		bool intersection = false;
		K::Point_2 inter_loc(0, 0);
		IP i_inter_loc(0, 0);


		std::vector<std::tuple<double, double, double, double>> points;
		for(int i=0; i < n; i++){
			double s; std::cin >> s;
			double t; std::cin >> t;
			double u; std::cin >> u;
			double v; std::cin >> v;
			points.push_back(std::make_tuple(s, t, u, v));
		}
		std::random_shuffle(points.begin(), points.end());
		for(int i=0; i < n; i++){
			double s = std::get<0>(points[i]);
			double t = std::get<1>(points[i]);
			double u = std::get<2>(points[i]);
			double v = std::get<3>(points[i]);
			IK::Point_2 p1(s, t);
			IK::Point_2 p2(u, v);
			IK::Segment_2 i_seg(p1, p2);
			if(CGAL::do_intersect(i_seg, i_ray)){
				auto result = CGAL::intersection(i_seg, i_ray);
				if(const IP* op = boost::get<IP>(&*result)){
					if(!intersection || CGAL::has_larger_distance_to_point(i_ray.source(), i_inter_loc, *op)){
						K::Point_2 e(s, t), f(u, v);
						K::Segment_2 seg(e, f);
						auto res = CGAL::intersection(seg, ray);
						const P* isec = boost::get<P>(&*res);
						inter_loc = *isec;
						i_inter_loc = *op;
					}
				}else if(const IS* op = boost::get<IS>(&*result)){
					if(!intersection){
						K::Point_2 e(s, t), f(u, v);
						K::Segment_2 seg(e, f);
						auto res = CGAL::intersection(seg, ray);
						const S* isec = boost::get<S>(&*res);
						inter_loc = (*isec).source();
						i_inter_loc = (*op).source();
					}
					if(CGAL::has_larger_distance_to_point(i_ray.source(), i_inter_loc, (*op).source())){
						K::Point_2 e(s, t), f(u, v);
						K::Segment_2 seg(e, f);
						auto res = CGAL::intersection(seg, ray);
						const S* isec = boost::get<S>(&*res);
						inter_loc = (*isec).source();
						i_inter_loc = (*op).source();
					}
					if(CGAL::has_larger_distance_to_point(i_ray.source(), i_inter_loc, (*op).target())){
						K::Point_2 e(s, t), f(u, v);
						K::Segment_2 seg(e, f);
						auto res = CGAL::intersection(seg, ray);
						const S* isec = boost::get<S>(&*res);
						inter_loc = (*isec).target();
						i_inter_loc = (*op).target();
					}
				}else{
					 throw std::runtime_error("Unknown Intersection type");
				}
				intersection = true;
			}
		}

		if(intersection){
			std::cout << (long long) floor_to_double(inter_loc.x()) << " " <<  (long long) floor_to_double(inter_loc.y()) << "\n";
		}else{
			std::cout << "no\n";
		}
		std::cin >> n;
	}

	std::cout << std::flush;
	return 0;
}
