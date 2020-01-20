#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Line_2 L;
typedef K::Ray_2 R;

using namespace std;

double floor_to_double(const K::FT & x) {

	double a = std::floor(CGAL::to_double(x));
	while (a > x) a-=1;
	while (a+1 <= x) a+=1;
	return a;
}

long floor_to_long(const K::FT & x) {
    return std::floor(CGAL::to_double(x));
}

void firsthit(int n, R &ray, vector<S> &obstacles) {

    bool start = true;
    P result;
    K::FT min_d;

    for (int i=0; i<n; i++) {

        //cout << i << " " << min_d << "\n";
        if (CGAL::do_intersect(ray, obstacles[i])) {

            auto inter = CGAL::intersection(ray, obstacles[i]);

            if (const P* op = boost::get<P>(&*inter)) {

                K::FT cand_d = CGAL::squared_distance(ray.source(), *op);            
                if (start) {
                    min_d = cand_d;
                    result = *op;
                    start = false;
                }
                if (cand_d < min_d) {
                    min_d = cand_d;
                    result = *op;
                }
            }
            else if (const S* os = boost::get<S>(&*inter)) {
                K::FT cand_d = CGAL::squared_distance(ray.source(), os->source());            
                if (start) {
                    min_d = cand_d;
                    result = os->source();
                    start = false;
                }
                if (cand_d < min_d) {
                    min_d = cand_d;
                    result = os->source();
                }
            }
            else cout << "strange intersection \n";
    

        }
    }

    if (start) {
        cout << "no\n";
    }
    else cout << floor_to_long(result.x()) << " " << floor_to_long(result.y()) << '\n';

}

int main(int argc, char const *argv[]) {

    ios_base::sync_with_stdio(false);

    int n;
    std::cin >> n;

    while(n != 0) {

        // read ray
        double x1, y1, x2, y2;
    
        std::cin >> x1 >> y1 >> x2 >> y2;

        P s1 = P(x1, y1);
        P s2 = P(x2, y2);
        R ray(s1, s2);

        // read segments obstacles
        vector<S> obstacles(n);

        for (int i=0; i<n; i++) {

            std::cin >> x1 >> y1 >> x2 >> y2;

            P p1 = P(x1, y1);
            P p2 = P(x2, y2);

            obstacles[i] = S(p1, p2);

        }

        // call solution function
        firsthit(n, ray, obstacles);
         
        std::cin >> n;
    }

    return 0;
}

