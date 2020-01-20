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

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;

}

void testcase(int n) {

    vector<P> people(n);
    for (int i = 0; i < n; i ++) {
        cin >> people[i];
    }

    K::FT squared_result;
    Min_circle mc(people.begin(), people.end(), true);
    squared_result = mc.circle().squared_radius();

    // try to remove every support point to improve squared_result
    int cpt = 0;
    for (auto it = mc.support_points_begin(); it != mc.support_points_end(); ++it) {
        cpt ++;
        P point_to_remove = *(it);

        for (int i = 0; i < n; i ++) {

            if (people[i] == point_to_remove) {

                people[i] = people[(i == 0 ? 1 : i - 1)];
                Min_circle mc(people.begin(), people.end(), true);
                squared_result = min(mc.circle().squared_radius(), squared_result);
                people[i] = point_to_remove;

                break;
            }
        }
    }
    cerr << cpt << "\n";
    cout << ceil_to_double(sqrt(squared_result)) << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    int n;
    cin >> n;
    while (n != 0) {
        testcase(n);
        cin >> n;
    }
}
