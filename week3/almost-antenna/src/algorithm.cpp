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

Ways to remove a support point from P in O(n). Here way iii. was used, but the others work too.
i - Use remove() which return iterator on the element after the last good one. 
But need to copy P at every iteration in a new vector
ii - when find element_to_remove, swap support_point with the last point in P. Then use P until the before last one.
iii - when find element_to_remove, replace support_point with a neighbor. Wont change result since a point will just be here 2 times.
After use, replace the element_to_remove at its place

Reason bugged so long with TLE: x and y long or double, are not int!!
*/


void testcase(int n) {

    vector<P2> P(n);
    for (int i = 0; i < n; i ++) {
        long x, y;
        cin >> x >> y;
        P[i] = P2(x, y);
    }

    K::FT squared_result;
    Min_circle mc(P.begin(), P.end(), true);
    squared_result = mc.circle().squared_radius();

    // try to remove every support point to improve squared_result
    for (auto support_point = mc.support_points_begin(); support_point < mc.support_points_end(); ++support_point) {
        //P2 point_to_remove = *(support_point);

        int i = 0;
        while (i < n) {

            if (P[i] == (*support_point)) {

                P[i] = P[(i == 0 ? 1 : i - 1)];
                Min_circle maybe(P.begin(), P.end(), true);
                K::FT maybeRadius = maybe.circle().squared_radius();
                if (squared_result > maybeRadius) {
                    squared_result = maybeRadius;
                }
                P[i] = (*support_point);

                break;
            }
            i ++;
        }
    }
    cout << ceil(sqrt(squared_result)) << "\n";

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
