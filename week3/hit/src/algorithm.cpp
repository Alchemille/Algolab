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

void inter(int n, R& ray, vector<S> &obstacles) {

    for (int i=0; i<n; i++) {
        if (CGAL::do_intersect(obstacles[i], ray)) {
            std::cout << "yes\n";
            return;
        };
    }
    std::cout << "no\n";
}

int main(int argc, char const *argv[]) {

    ios_base::sync_with_stdio(false);

    int n;
    std::cin >> n;

    while(n != 0) {
    //for (int i=0; i<7; i++) {

        // read ray
        double x1, y1, x2, y2;
    
        std::cin >> x1 >> y1 >> x2 >> y2;

        P p1 = P(x1, y1);
        P p2 = P(x2, y2);
        R ray(p1, p2);

        // read segments obstacles
        vector<S> obstacles(n);

        for (int i=0; i<n; i++) {

            std::cin >> x1 >> y1 >> x2 >> y2;

            P p1 = P(x1, y1);
            P p2 = P(x2, y2);

            obstacles[i] = S(p1, p2);
        }

        // call solution function
        inter(n, ray, obstacles);
        
        //cout << n << " " << x1 << " " << y1 << " " << x2 << " " << y2 <<  "\n";
 
        std::cin >> n;
    }

    return 0;
}
