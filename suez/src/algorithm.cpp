#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

/*
Fast version:
Not set constraint for all (new, old) pair, but only (new, closest_old).
Find the threatening axis (x or y) and record threatening distance on the axis.
Closest old is the old that has the shortest threatening distance.
For (new, new):
Find threatening axis and set constraint on this axis

Carefult to:
IT of LP is super crucial
type of other ppl is also super crucial. eg line 76-77: wont work if set long instead of IT
Division of ints/longs
cout << setprecsion

*/

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double ceil_to_double(CGAL::Quotient<ET> x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcase() {

    Program lp(CGAL::SMALLER, true, 1, false, 0); // scaling factor a >= 1
    int n, m, h, w;
    cin >> n >> m >> h >> w;
    int constraints_counter = 0;

    vector<long> x_new(n, 0);
    vector<long> y_new(n, 0);
    vector<long> x_old(m, 0);
    vector<long> y_old(m, 0);

    for (int i = 0; i < n; i++) {
        cin >> x_new[i] >> y_new[i];
    }
    for (int i = 0; i < m; i++) {
        cin >> x_old[i] >> y_old[i];
    }

    // For each new poster, for each old, define exactly 1 constraint
    if (m > 0) {
        for (int i = 0; i < n; i++) {

            // new poster
            long x_n = x_new[i];
            long y_n = y_new[i];

            IT min_ratio_x = INT_MAX;
            IT min_ratio_y = INT_MAX;

            for (int j = 0; j < m; j++) {

                // old poster
                IT x_o = x_old[j]; // weird: if long, wont work 0_0
                IT y_o = y_old[j];

                IT ratio_x = abs(x_n - x_o) / w;
                IT ratio_y = abs(y_n - y_o) / h;

                if (ratio_x >= ratio_y) { // If we move faster towards the edge on the x-axis
                    min_ratio_x = min(min_ratio_x, ratio_x);
                } else {
                    min_ratio_y = min(min_ratio_y, ratio_y);
                }

            }

            lp.set_a(i, constraints_counter, 1);
            lp.set_b(constraints_counter, 2 * min(min_ratio_x, min_ratio_y) - 1);
            constraints_counter++;

        }
    }

    // For each new poster, for each new, define exactly 1 constraint
    // 1 constraint per distinct pair
    for (int i = 0; i < n; i++) {

        // now poster
        long x_n = x_new[i];
        long y_n = y_new[i];

        lp.set_c(i, -2 * (w + h)); // translate maximization problem as a minimization one

        for (int j = i + 1; j < n; j++) {
            // other new poster
            long x_o = x_new[j];
            long y_o = y_new[j];

            long dx = abs(x_n - x_o);
            long dy = abs(y_n - y_o);

            if (dx * h >= dy * w) { 
                lp.set_a(i, constraints_counter, w);
                lp.set_a(j, constraints_counter, w);
                lp.set_b(constraints_counter, 2 * abs(x_n - x_o));
            } else {
                lp.set_a(i, constraints_counter, h);
                lp.set_a(j, constraints_counter, h);
                lp.set_b(constraints_counter, 2 * abs(y_n - y_o));
            }

            constraints_counter++;
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    assert(s.solves_linear_program(lp));

    cerr << s;

    cout << ceil(CGAL::to_double((-s.objective_value()))) << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(10);
    int t;
    cin >> t;
    while (t--) testcase();
}