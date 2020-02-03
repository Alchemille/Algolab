#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <assert.h>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/Gmpzf.h>

typedef CGAL::Gmpzf IT;
typedef CGAL::Gmpzf ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

/*
https://www.geeksforgeeks.org/inline-functions-cpp/
Didnt have right formulation pour polynome a plusieurs indeterminees:
every x^i * y^j * z^k with i + j + k <= d has its own coefficient

Revelation binary search
lp.set_r for to set relation to >= for a constraint
vital: check if degree should be 0 before binary search
vital: Gmpzf and not Gmpz
*/

struct particle {
    double x;
    double y;
    double z;
};

vector<double> coeff(double d , double x, double y, double z) {
    // no need to record the order of monomials in the polynomial
    // important that same order of monomials in vector for every function call = constraint
    // each value in vector returned is associated with a different polynomial coefficient

    vector<double> result;
    for (int i = 0; i <= d; i ++) {
        for (int j = 0; j <= d - i; j ++) {
            for (int k = 0; k <= d - i - j; k ++) {
                result.push_back(pow(x, i) * pow(y, j) * pow(z, k));
            }
        }
    }

    return result;
}

bool too_small(int d, int h, int t, vector<particle>& healthy, vector<particle>& tumor) {

    // test whether heathy and tumor cells can be linearly separated with polynomial of degree d
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    // heathy cells constraints
    for (int i = 0; i < h; i ++) {

        // compute polynomial values associated with coeffs to fill A for this constraint
        vector<double> poly_values = coeff(d, healthy[i].x, healthy[i].y, healthy[i].z);

        // each poly_value is associated with 1 coefficient = LP variable
        for (int j = 0; j < poly_values.size(); j ++) {
            lp.set_a(j, i, poly_values[j]);
        }

        // margin constraint
        lp.set_r(i, CGAL::SMALLER);
        lp.set_b(i, -1);
    }

    // tumor cells constraints
    for (int i = 0; i < t; i ++) {

        // compute polynomial values associated with coeffs to fill A for this constraint
        vector<double> poly_values = coeff(d, tumor[i].x, tumor[i].y, tumor[i].z);

        // each poly_value is associated with 1 coefficient = LP variable
        for (int j = 0; j < poly_values.size(); j ++) {
            lp.set_a(j, h + i, poly_values[j]);
        }

        lp.set_r(h+i, CGAL::LARGER);
        lp.set_b(i + h, 1); 
    }

    // solve the program, using ET as the exact type
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    assert(s.solves_linear_program(lp));

    return s.is_infeasible();
}

void testcase() {

    int h, t;
    cin >> h >> t; 
    vector<particle> healthy(h);
    vector<particle> tumor(t);


    for (int i = 0; i < h; i ++) {
        cin >> healthy[i].x >> healthy[i].y >> healthy[i].z;
    }
    for (int i = 0; i < t; i ++) {
        cin >> tumor[i].x >> tumor[i].y >> tumor[i].z;
    }
    
    // binary search to find degee
    int a = 1;
    int b = 2;
    int LIM = 30;

    if(h == 0 || t == 0) { // vital, WA and TEL otherwise !!!
        cout << 0 << endl;
        return;
    }

    // try to make b smaller than 30
    while(b <= LIM && too_small(b, h, t, healthy, tumor)) {
        a = b + 1;
        b = min(LIM, b * 2 + 1);
    }

    while (a != b) {
        int m = (b + a) / 2;
        if (too_small(m, h, t, healthy, tumor)) {
            a = m + 1;
        }
        else {
            b = m;
        }
    }

    if (b > LIM) {
        cout << "Impossible!\n";
        return;
    }

    cout << a << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
