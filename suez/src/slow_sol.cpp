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
1 constraint per (new, old) and one per (new, new).
Divide area around new in 4 zones, delimited by 4 lines out of corner.
Compare slopes to see in what area is other poster.
Depending on the area, we know what side of other poster is threatening,
and can define the constraint

Segfault could be caused by a vector that was not assigned a size
Careful about IT and ET in LP, and divisions of ints
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
    
    Program lp (CGAL::SMALLER, true, 1, false, 0); // scaling factor a >= 1
    int n, m, h, w;
    cin >> n >> m >> h >> w;
    int constraints_counter = 0;

    IT half_w = w/2.;
    IT half_h = h/2.;

    vector<long> x_new(n, 0);
    vector<long> y_new(n, 0);
    vector<long> x_old(m, 0);
    vector<long> y_old(m, 0);

    for (int i=0; i<n; i++) {
        cin >> x_new[i] >> y_new[i];
    }
    for (int i=0; i<m; i++) {
        cin >> x_old[i] >> y_old[i];
    }

    // For each new poster, for each old, define exactly 1 constraint
    for (int i=0; i<n; i++) { 

        // new poster
        long x_n = x_new[i];
        long y_n = y_new[i];

        for (int j=0; j<m; j++) {

            // old poster
            long x_o = x_old[j];
            long y_o = y_old[j];

            if (y_o > y_n + half_h && (x_o == x_n || h * abs(x_o - x_n) <= w * abs(y_o - y_n))) { 
                // zone above, h / w <= |dy / dx|
                lp.set_a(i, constraints_counter, half_h);
                lp.set_b(constraints_counter, -y_n + y_o - half_h); 

                //cout << i << " " << j << " zone 1 \n"; 
            }
            else if (x_o > x_n + half_w && (y_o == y_n || h * abs(x_o - x_n) >= w * abs(y_o - y_n))) { 
                // zone right, |dy / dx| <= h / w
                lp.set_a(i, constraints_counter, half_w);
                lp.set_b(constraints_counter, -x_n + x_o - half_w);

                //cout << i << " " << j << " zone 2 \n"; 
            }
            else if (y_o < y_n - half_h && (x_o == x_n || h * abs(x_o - x_n) <= w * abs(y_o - y_n))) { 
                // zone below, |dy / dx| <= h / w
                lp.set_a(i, constraints_counter, half_h);
                lp.set_b(constraints_counter, y_n - y_o - half_h);

                //cout << i << " " << j << " zone 3 \n"; 
            }
            else if (x_o < x_n - half_w && (y_o == y_n || h * abs(x_o - x_n) >= w * abs(y_o - y_n))) { 
                // zone left, h / w <= |dy / dx|
                lp.set_a(i, constraints_counter, half_w);
                lp.set_b(constraints_counter, x_n - x_o - half_w);

                //cout << i << " " << j << " zone 4 \n"; 
            }

            else cerr << "no zone! \n";

            constraints_counter ++;
        }
    }

    // For each new poster, for each new, define exactly 1 constraint
    // 1 constraint per distinct pair
    for (int i=0; i<n; i++) {

        // now poster
        long x_n = x_new[i];
        long y_n = y_new[i];

        lp.set_c(i, -1); // translate maximization problem as a minimization one

        for (int j=i + 1; j<n; j++) {
            
            // other new poster
            long x_o = x_new[j];
            long y_o = y_new[j];

            if (y_o > y_n + half_h && (x_o == x_n || h * abs(x_o - x_n) <= w * abs(y_o - y_n))) { // zone above
                
                //cout << i << " " << j << " zone 1 \n"; 
                
                lp.set_a(i, constraints_counter, half_h);
                lp.set_a(j, constraints_counter, half_h);
                lp.set_b(constraints_counter, -y_n + y_o); 
            }
            else if (x_o > x_n + half_w && (y_o == y_n || h * abs(x_o - x_n) >= w * abs(y_o - y_n))) { // zone right
                lp.set_a(i, constraints_counter, half_w);
                lp.set_a(j, constraints_counter, half_w);
                lp.set_b(constraints_counter, -x_n + x_o);

                //cout << i << " " << j << " zone 2 \n"; 
            }
            else if (y_o < y_n - half_h && (x_o == x_n || h * abs(x_o - x_n) <= w * abs(y_o - y_n))) { // zone below
                lp.set_a(i, constraints_counter, half_h);
                lp.set_a(j, constraints_counter, half_h);
                lp.set_b(constraints_counter, y_n - y_o);

                //cout << i << " " << j << " zone 3 \n"; 
            }
            else if (x_o < x_n - half_w && (y_o == y_n || h * abs(x_o - x_n) >= w * abs(y_o - y_n))) { // zone left
                lp.set_a(i, constraints_counter, half_w);
                lp.set_a(j, constraints_counter, half_w);
                lp.set_b(constraints_counter, x_n - x_o);

                //cout << i << " " << j << " zone 4 \n"; 
            }
            else cerr << "no zone! \n";

            constraints_counter ++;
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    cerr << s;
    cout << ceil(CGAL::to_double(2 * (w + h) * -s.objective_value())) << "\n"; // sum of perimeters of new posters
    
    // for (auto it = s.variable_values_begin(); it != s.variable_values_end(); ++it) {
    //     cout << *it << " ";
    // }
    // cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    std::cout << std::setprecision(15);
    int t;
    cin >> t;
    while (t--) testcase();
}
