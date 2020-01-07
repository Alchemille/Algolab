#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

/*
idea (no triangulation) in n*n:
Put all planets in a UF
Start from last planet not taken by empire. 
For each new planet accessible (day i--) :
    rebels can have at most i planets
    update CCs with the new planet (other double loop)
    count maximal cc size that is smaller than the upperbound
return maximal maximal cc size


rappels:
- pow(r, 2) and not r * r
- iteration over maps -> paire
- number of for loops does matter
*/

void testcase() {

    int n, r;
    cin >> n >> r;
    vector<P> planets(n);

    for (int i = 0; i < n; i ++) {
        int x, y;
        cin >> x >> y;
        planets[i] = {x, y};
    }

    boost::disjoint_sets_with_storage<> uf(n);
    vector<int> n_cc(n, 1);
    int result = 0;

    for (int i = n-1; i > 0; i --) { // i is the number of planets controlled by the emperor. First planet never accessible

        // update the CCs of the set of nice planets
        int max_cc_day = 0;
        for (int j = n - 1; j >= i; j --) {

            int cj = uf.find_set(j);
            int ci = uf.find_set(i);

            if (CGAL::squared_distance(planets[i], planets[j]) <= pow(r, 2) && ci != cj) {

                uf.link(ci, cj);

                if (uf.find_set(j) == cj) {
                    n_cc[cj] += n_cc[ci];
                    n_cc[ci] = 0;
                    max_cc_day = max(n_cc[cj], max_cc_day);
                }
                else {
                    n_cc[ci] += n_cc[cj];
                    n_cc[cj] = 0;
                    max_cc_day = max(n_cc[ci], max_cc_day);
                } 
            }
        }
        
        if (max_cc_day > i) max_cc_day = i;
        result = max(max_cc_day, result);
    }

    cout << result << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}