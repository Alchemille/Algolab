#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <assert.h>
#include <cassert>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase1(int a, int b) {

    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    const int X = 0;
    const int Y = 1;
    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_a(X, 1, 4);
    lp.set_a(Y, 1, 2);
    lp.set_a(X, 2, -1);
    lp.set_a(Y, 2, 1);

    lp.set_b(0, 4);
    lp.set_b(1, a*b);
    lp.set_b(2, 1);

    lp.set_c(X, a);
    lp.set_c(Y, -b);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // output solution
    if (s.is_infeasible() || s.is_unbounded()) cout << "no\n";
    else {
        int sol = floor(-CGAL::to_double(s.objective_value()));
        cout << sol << "\n";
    }

}

void testcase2(int a, int b) {
    Program lp (CGAL::SMALLER, false, 0, true, 0);
    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    lp.set_a(X, 0, -1);
    lp.set_a(Y, 0, -1);
    
    lp.set_a(X, 1, -4);
    lp.set_a(Y, 1, -2);
    lp.set_a(Z, 1, -1);

    lp.set_a(X, 2, 1);
    lp.set_a(Y, 2, -1);
    lp.set_a(Z, 2, 0);
    
    lp.set_b(0, 4);
    lp.set_b(1, a*b);
    lp.set_b(2, 1);

    lp.set_c(X, a);
    lp.set_c(Y, b);
    lp.set_c(Z, 1);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // output solution
    if (s.is_infeasible() || s.is_unbounded()) cout << "no\n";
    else {
        int sol = ceil(CGAL::to_double(s.objective_value()));
        cout << sol << "\n";
    }
}


int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int p, a, b;
    cin >> p;
    while (p!=0) {
        cin >> a >> b;
        if (p==1) testcase1(a, b);
        else testcase2(a, b);
        cin >> p;
    }
}
