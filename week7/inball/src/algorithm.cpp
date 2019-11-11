#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <math.h> 
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;

using namespace std;

void testcase(int n, int d) {

    int a, b;

    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    for (int i=0; i<n; i++) {

        long sum_squared = 0; // to compute euclidian norm of hyperplane
        for (int j=0; j<d; j++) {
            cin >> a;
            sum_squared += a*a;
            lp.set_a(j, i, a); // n first constraints: c inside the polyhedron AX <= b
            lp.set_a(j, n+i, a); // n next constraints: distance from c to hyperpane >= c
        }
        lp.set_a(d, i, 0); // column d (last) is for variable r in n first consstraints
        int norm_hyp =  sqrt(sum_squared);
        lp.set_a(d, n+i, norm_hyp); // column d (last) for r in n next constraints

        cin >> b;
        lp.set_b(i, b);
        lp.set_b(n+i, b);
    }

    // r (last variable) should be > 0
    lp.set_l(d, true, 0); 

    // set vector c: only activate variable d which corresponds to center c
    for (int i=0; i<d; i++) {
        lp.set_c(i, 0);
    }
    lp.set_c(d, -1); // minus to transfor max problem into min problem

    // Solve LP
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_infeasible()) cout << "none\n";
    else if (s.is_unbounded()) cout << "inf\n";
    else cout << floor(CGAL::to_double(-s.objective_value())) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n, d;
    cin >> n;
    while (n != 0) {
        cin >> d;
        testcase(n, d);
        cin >> n;
    }
    
}
