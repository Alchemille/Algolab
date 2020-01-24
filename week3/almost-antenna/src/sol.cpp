#include <iostream>
#include <cmath>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 P2;

using namespace std;

/*
https://www.fluentcpp.com/2018/09/14/how-to-remove-elements-from-a-sequence-container/
https://www.geeksforgeeks.org/difference-between-stdremove-and-vectorerase-for-vectors/
https://www.geeksforgeeks.org/ways-copy-vector-c/

trick: 
only try to remove support points (at most 3), not all points.

Ways to remove a support point from people in O(n). Here way iii. was used, but the others work too.
i - Use remove() which return iterator on the element after the last good one. 
But need to copy people at every iteration in a new vector
ii - when find element_to_remove, swap it with the last point in people. Then use people until the before last one.
iii - when find element_to_remove, replace it with a neighbor. Wont change result since a point will just be here 2 times.
After use, replace the element_to_remove at its place
*/


void testcase(int n) {

    // Read input points
    std::vector<P2> P(n);
    for(int i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        P[i] = P2(x, y);
    }

    // Compute min bounding circle around all the points
    Min_circle mc1(P.begin(), P.end(), true);
    K::FT minRadius = mc1.circle().squared_radius();
    // Try removing every support point and see which result gives the smallest radius

    for(auto support_point = mc1.support_points_begin(); support_point < mc1.support_points_end(); ++support_point) {

        int i = 0; 
        while (i < n) {
            if (P[i] == *support_point) {
                // "Delete" a support point
                P[i] =  P[i + (i == 0 ? 1 : -1)];
                Min_circle maybe(P.begin(), P.end(), true);
                K::FT maybeRadius = maybe.circle().squared_radius();
                if (minRadius > maybeRadius) {
                    minRadius = maybeRadius;
                }
                P[i] =  *support_point;
                break;
            }
            i++;
        }
    }

    std::cout << ceil(sqrt(minRadius)) << std::endl;

}

int main(int argc, char const *argv[]) {
    std::cout << fixed << std::setprecision(0);
    int n;
    cin >> n;
    while (n != 0) {
        testcase(n);
        cin >> n;
    }
}
