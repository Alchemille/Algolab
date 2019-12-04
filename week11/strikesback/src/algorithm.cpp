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
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef K::Point_2 P;

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

/*
3 steps:
i_ determine maximal radius of each shot with delaunay
ii_ determine set of shots for each particle
iii_ LP to minimize energy required to kill everyone

careful to
- segfault could be caused by existence pb, eg call t.nearest_vertex when t has no vertex
- LP: opposite of smaller is larger

*/

struct particle {
    P p;
    int d;
};

void testcase() {

    // read and store input
    int a, s, b, e;
    cin >> a >> s >> b >> e;

    vector<particle> particles(a);
    for (int i = 0; i < a; i ++) {
        int x, y, d;
        cin >> x >> y >> d;
        particles[i] = particle({P(x, y), d});
    }

    vector<P> shots(s);
    for (int i = 0; i < s; i ++) {
        int x, y;
        cin >> x >> y;
        shots[i] = P(x, y);
    }

    vector<P> bounties(b);
    for (int i = 0; i < b; i ++) {
        int x, y;
        cin >> x >> y;
        bounties[i] = P(x, y);
    }

    // i_ triangulation to find radius (squared) of each shot
    Triangulation t;
    t.insert(bounties.begin(), bounties.end());

    vector<K::FT> radii_shots(s);
    for (int i = 0; i < s; i ++) {

        if (b != 0) { // important to make sure at leat 1 vertex in t. segfault on line below otherwise
            P hunter = t.nearest_vertex(shots[i])->point();
            radii_shots[i] = CGAL::squared_distance(hunter, shots[i]);
        }
        else radii_shots[i] = LONG_MAX; // no bountie hunters
    }

    // ii_ for each particle, find shots that affect it
    vector<vector<pair<int, K::FT>>> affectation(a, vector<pair<int, K::FT>>());

    for (int i = 0; i < a; i ++) {
        for (int j = 0; j < s; j ++) {

            K::FT distance = CGAL::squared_distance(particles[i].p, shots[j]);

            if (distance < radii_shots[j]) { // particle i is affected by shot j
                affectation[i].push_back({j, distance});
            }
        }
    }

    // iii_ formulate and solve LP. s variables and a constraints. minimization
    Program lp (CGAL::LARGER, true, 0, false, 0); 

    // set vector c
    for (int i = 0; i < s; i ++) {
        lp.set_c(i, 1); 
    }

    // set A and b
    for (int i = 0; i < a; i ++) {
        
        lp.set_b(i, particles[i].d);

        for (auto it = affectation[i].begin(); it != affectation[i].end(); ++it) {
            K::FT a_i_it = 1. / std::max(1., (*it).second);
            lp.set_a((*it).first, i, a_i_it);
        }
    }

    // solve lp
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));

    if (sol.is_infeasible() || sol.objective_value() > e) { // not use to_double(), precision error otherwised
        cout << "n\n";
    }
    else {
        cout << "y\n";
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
