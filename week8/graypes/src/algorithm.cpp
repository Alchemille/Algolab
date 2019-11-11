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
typedef K::Point_2 P;

using namespace std;

/*
https://doc.cgal.org/latest/BGL/index.html#title15
https://doc.cgal.org/latest/Triangulation_2/classCGAL_1_1Triangulation__2.html

!! Careful about finite VS all iterators of triangulation
!! Careful when comparing FT and regular types. Delay conversion to the latest

To get len of edge:
access vertices and compute distance with squared_distance()
OR
converte edge to segment and compute distance with squared_length()
https://github.com/groggi/eth-algolab/blob/master/graypes/main.cpp
https://github.com/telmich/algolab/blob/master/graypes.cpp

*/

void testcase(int n) {

    // read points
    vector<P> pts(n);
    int x, y;

    for (int i=0; i<n; i++) {
        cin >> x >> y;
        pts[i] = P(x, y);
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // find the shortest edge in the Delaynay triangulation
    K::FT distance;
    bool first = true;

    for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); ++it) {

        K::FT dist = t.segment(it).squared_length();
        if (first) {
            distance = dist;
            first = false;
        }
        else distance = min(distance, dist);
    }

    // at the last moment, get out from FT domain, compute sqrt and find time from distance + speed;
    double result = ceil(sqrt(CGAL::to_double(distance)) * 50);
    cout << result << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while (n != 0) {
        testcase(n);
        cin >> n;
    }
}
