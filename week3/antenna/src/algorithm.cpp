#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Line_2 L;
typedef K::Ray_2 R;

using namespace std;

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;

}

void antenna(int n, vector<P> houses) {

    // find minimum circle
    Min_circle mc(houses.begin(), houses.end(), true);

    // convert min circle to circle
    Traits::Circle c = mc.circle();
    cout << ceil_to_double(sqrt(c.squared_radius())) << "\n";

}

int main(int argc, char const *argv[]) {

    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);

    int n;
    std::cin >> n;

    while(n != 0) {

        double x, y;
    
        // read houses coordinates
        vector<P> houses(n);

        for (int i=0; i<n; i++) {

            std::cin >> x >> y;
            houses[i] = P(x, y);

        }

        // call solution function
        antenna(n, houses);
         
        std::cin >> n;
    }

    return 0;
}
