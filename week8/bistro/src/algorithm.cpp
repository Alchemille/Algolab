#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 P;

using namespace std;

/*
Idea: cut into Voronoi region, that correspond to a Delaunay vertex. Like post office.
useful: t.segment(e), vertex_handle->point()

Careful with handles often returned (eg by nearest_vertex or iterator or vertex accessed through edge representation)
Careful to setprecision when dealing with distance -_-
*/

void testcase(int n) {

    // read points
    vector<P> pts(n);
    int x, y;
    //cout << n << "\n";

    for (int i=0; i<n; i++) {
        cin >> x >> y;
        pts[i] = P(x, y);
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // read query points
    int m;
    cin >> m;
    //cout << m << "\n";

    for (int i=0; i<m; i++) {

        cin >> x >> y;
        P p1 = P(x, y);
        Vertex u = t.nearest_vertex(p1);
        P p2 = u->point();

        cout << CGAL::squared_distance(p1, p2) << "\n";
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << setprecision(15); // super important !!
    int n;
    cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
}
