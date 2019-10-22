#include <iostream>
#include <stdexcept>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <cmath>
#include <climits>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;


double floor_to_double(const K::FT & x) {

	double a = std::floor(CGAL::to_double(x));
	while (a > x) a-=1;
	while (a+1 <= x) a+=1;
	return a;
}


int main() {

	ios_base::sync_with_stdio(false);	
	int n, intersected, first;
    double r,s,t,u,x,y,a,b;
	K::FT min_distance, distance;
    K::Point_2 p1,p2;
    K::Point_2 o1,o2;
	K::Point_2 min_point;
    K::Segment_2 obstacle;
    K::Ray_2 ray;

	std::cin >> n;
	while (n != 0) {
        intersected = 0;
		first = 0;
        std::cin >> x;
        std::cin >> y;
        std::cin >> a;
        std::cin >> b;
        p1 = K::Point_2(x, y);
        p2 = K::Point_2(a, b);
        ray = K::Ray_2(p1, p2);
//		cout << "Starting point: " << p1 << endl;
        for (int i=0; i<n; i++) {
            std::cin >> r;
            std::cin >> s;
            std::cin >> t;
            std::cin >> u;
            o1 = K::Point_2(r, s);
            o2 = K::Point_2(t, u);
			//If segment
            //Check for intersection
			cout << std::fixed << std::setprecision(0);
            	obstacle = K::Segment_2(o1, o2);
//				cout << "Now checking for obstacle : " << obstacle << endl;
            	if (CGAL::do_intersect(ray, obstacle)){
					auto o = CGAL::intersection(ray, obstacle);
					if (const P* op = boost::get<P>(&*o)){
						//Point intersection
						distance = CGAL::squared_distance(*op, p1); 
						if (first == 0){
							min_distance = distance;
							first = 1;
						}
//						cout << "Distance for  point " << *op << " = " << distance << endl;
						if (distance <= min_distance) {
//							cout << "Distance is < min_distance = " << min_distance << endl;
							min_distance = distance;
							min_point = *op;
						}
						//cout << floor_to_double(op->x()) << " " << floor_to_double(op->y()) << endl;
						//cout << op->x() << " " << op->y() << endl;

					}
					else if (const S* os = boost::get<S>(&*o)){
						K::Point_2 source(os->source());
						distance = CGAL::squared_distance(source, p1); 
						if (first == 0){
							min_distance = distance;
							first = 1;
						}
						if (distance <= min_distance) {
							min_distance = distance;
							min_point = source;
						}
					}
                	intersected = 1;
            	}

        }
	
        if (intersected == 0)
            cout << "no" << endl;
		else
			cout << floor_to_double(min_point.x()) << " " << floor_to_double(min_point.y()) << endl;

        std::cin >> n;
    }

    return 0;
}