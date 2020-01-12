#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <assert.h>
#include <bitset>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/dynamic_bitset.hpp> // tu use dynamic bitsets

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef K::Point_2 P;

using namespace std;

/*
- set_r(i, CGAL::EQUAL) to ask for equality of constraint i
- remember that in set_a, first variable and then constraint indices
- careful division ints

To count number of lines between each pair in complexity L * W * S, can do 
- triangulation of (W + S) and look at nearest_vertex from centers of lines (W*S*100)
- difference symetrique (W*S*100)
- dynamic bitsets (XOR then count)

Documentation about bitsets (need include above):
https://www.boost.org/doc/libs/1_43_0/libs/dynamic_bitset/dynamic_bitset.html
https://www.boost.org/doc/libs/1_66_0/libs/dynamic_bitset/example/example1.cpp
https://www.geeksforgeeks.org/c-boostdynamic_bitset-class-with-examples/
https://stackoverflow.com/questions/4048749/bitwise-operations-on-vectorbool
https://stackoverflow.com/questions/14204844/c-adding-two-bool-arrays
https://stackoverflow.com/questions/1472048/how-to-append-a-char-to-a-stdstring
*/

double floor_to_double(const CGAL::Quotient<ET> x)
{
  double a = std::floor(CGAL::to_double(x));
  while(a+1<=x) ++a;
  while(a>x) --a;
  return a;
}

void testcase() {

    int W, S, L; 
    cin >> W >> S >> L;
    Program lp (CGAL::SMALLER, true, 0, false, 0);  // flows are positive

    // W first constraints: supply in liter of each warehouse
    vector<P> pos_w(W);
    vector<int> alcohol_perc(W, 0);
    for (int i = 0; i < W; i ++) {

        int x, y, s, a;
        cin >> x >> y >> s >> a;
        pos_w[i] = P(x, y);
        alcohol_perc[i] = a;

        // supply constraint for the warehouse
        for (int j = 0; j < S; j ++) {
            lp.set_a(i * S + j, i, 1);
        }
        lp.set_b(i, s);
    }

    // 2 * S next constraints: demand in liter of each stadium (equality constraint)
    vector<P> pos_s(S);
    vector<int> alcohol_upper(S, 0);
    for (int i = 0; i < S; i ++) {

        int x, y, d, u;
        cin >> x >> y >> d >> u;
        pos_s[i] = P(x,y);
        alcohol_upper[i] = u;
        
        for (int j = 0; j < W; j ++) {
            lp.set_a(j * S + i, W + i, 1);
        }
        lp.set_b(W + i, d);
        lp.set_r(W + i, CGAL::EQUAL); // constraint of equality
    }

    // S constraints: upperbound of alcohol
    for (int i = 0; i < S; i ++) {
        for (int j = 0; j < W; j ++) {
            lp.set_a(j * S + i, W + S + i, alcohol_perc[j]);
        }
        lp.set_b(W + S + i, 100 * alcohol_upper[i]);
    }

    // read rest of input
    
    vector<int> radii(L);
    vector<P> centers(L);
    vector<vector<int>> revenues(W, vector<int>(S, 0));
    for (int i = 0; i < W; i ++) {
        for (int j = 0; j < S; j ++) {
            cin >> revenues[i][j];
        }
    }    
    for (int i = 0; i < L; i ++) {
        cin >> centers[i] >> radii[i];
    }

    // compute number of lines between all pairs (w, s)
    vector<vector<int>> n_lines(W, vector<int>(S, 0));
    vector<boost::dynamic_bitset<>> lines_w(W, boost::dynamic_bitset<>(L)); // L bits set to false(0)
    vector<boost::dynamic_bitset<>> lines_s(S, boost::dynamic_bitset<>(L)); // L bits set to false(0)

    // for each w or s, check in which line is contained
    for (int i = 0; i < L; i ++) {
        int r = radii[i];
        for (int w = 0; w < W; w ++) {
            lines_w[w][i] = CGAL::squared_distance(pos_w[w], centers[i]) <= pow(r, 2);
        }
        for (int s = 0; s < S; s ++) {
            lines_s[s][i] = CGAL::squared_distance(pos_s[s], centers[i]) <= pow(r, 2);
        }
    }

    // for each pair(s, w), check which lines contain only one of them. use XOR operator and then count
    boost::dynamic_bitset<> tmp(L);
    for (int w = 0; w < W; w ++) {
        for (int s = 0; s < S; s ++) {
            tmp.operator = (lines_w[w]);
            tmp.operator^= (lines_s[s]);    
            n_lines[w][s] = tmp.count();
        }
    }

    // set vector c with revenues, taking into account number of lines between (w, s)
    for (int i = 0; i < W; i ++) {
        for (int j = 0; j < S; j ++) {
            double profit = revenues[i][j] - (n_lines[i][j] / 100.);
            lp.set_c(i * S + j, -profit); // transform to min LP
        }
    }

    // compute LP
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if (s.is_infeasible()) cout << "RIOT!\n";
    else cout << floor_to_double(-s.objective_value()) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);
    int t;
    cin >> t;
    while (t--) testcase();
}
