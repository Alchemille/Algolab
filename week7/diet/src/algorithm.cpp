#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <cassert>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase(int n, int m) {
    
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    vector<int> A(n, 0); // lower bound for each nutrient
    vector<int> B(n, 0); // upper bound for each ntrient
    vector<int> C(m, 0); // prices for each unit

    for (int i=0; i<n; i++) {
        cin >> A[i] >> B[i];
    }

    // fill vector b of size 2n
    for (int i=0; i<n; i++) {
        lp.set_b(i, B[i]);
        lp.set_b(n + i, -A[i]);
    }

    int c, quantity_aliment;
    // fill matrix dim 2n * m
    for (int i=0; i<m; i++) {
        cin >> c;
        lp.set_c(i, c);
        // fill column of LP matrix.

        for (int j=0; j<n; j++) { // Product i, nutrient j
            cin >> quantity_aliment;
            lp.set_a(i, j, quantity_aliment);
            lp.set_a(i, n + j, -quantity_aliment);
        }
    }

    
    // Solve LP
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_infeasible()) cout << "No such diet.\n";
    else cout << floor(CGAL::to_double(s.objective_value())) << "\n";
    //cerr << s << "\n";
    //cerr << s.number_of_basic_constraints() << " " << s.number_of_basic_variables() << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    while (n!=0 || m!=0) {
        testcase(n, m);
        cin >> n >> m;
    }
}
