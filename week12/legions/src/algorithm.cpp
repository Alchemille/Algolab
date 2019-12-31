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
strategy: 
max min problem
max Z, Tl <= Z for every legion l. Z is the time of the first region arrival. Tl is the arrival time of each legion l.
precompute sign of each constraint

Weird things noticed:
- Whith only the sign constraints, the problem (sample.in) is infeasible which is not possible !
- the formula for distance dans l'enonce is weird cause could be negative !

Trick:
Need to use abs for distance !
abs (x) = x * sign(x)
a * a + b * b led to numerical errors but pow(a, 2) was fine
not forget that in (CGAL::SMALLER, false, 0, false, 0), its about each Ax <= b and not the objective funtion
*/

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase() {

    // read inputs
    long x, y, n;
    cin >> x >> y >> n;

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    // 2 constraints per roman line: sign constraint and max min constraint
    for (int i = 0; i < n; i ++) {

        int a, b, c, v, int_sign;
        cin >> a >> b >> c >> v;
        double sign = a * x + b * y + c;

        // set constraint sign
        if (sign < 0) { // ax + by <= -c
            int_sign = -1;

            lp.set_a(0, 2 * i, a);
            lp.set_a(1, 2 * i, b);
            lp.set_a(2, 2 * i, 0);
            lp.set_b(2 * i, - c);
        }
        else { // -ax -by <= c
            int_sign = 1;

            lp.set_a(0, 2 * i, -a);
            lp.set_a(1, 2 * i, -b);
            lp.set_a(2, 2 * i, 0);
            lp.set_b(2 * i, c);
        }

        // min max constraint. Z <= (ax + by + c) / tmp 
        long long tmp = v * sqrt(pow(a, 2) + pow(b, 2));
        //cerr << v << " " << tmp << " " << sqrt(a * a + b * b) << " " << a << " " << b << " " << a * a + b * b << "\n";

        lp.set_a(0, 2 * i + 1, - a * int_sign);
        lp.set_a(1, 2 * i + 1, - b * int_sign);
        lp.set_a(2, 2 * i + 1, tmp);

        lp.set_b(2 * i + 1, c * int_sign);
    }

    lp.set_c(0, 0);
    lp.set_c(1, 0);
    lp.set_c(2, -1); // since maximization problem

    // Solve LP
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    cout << floor(CGAL::to_double(-s.objective_value())) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
