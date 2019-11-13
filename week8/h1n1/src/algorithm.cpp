#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>
#include <unordered_map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Face_handle Face;
typedef K::Point_2 P;

using namespace std;

/*
Tricks: 
- Check if first position is contaminated: not sufficient to look at 3 vertices,
also look at nearest vertex (nlogn)
- not necessary to test if other positions are contaminated with distance with 3 vertices.
Thanks to properties of Delaunay triangulation.
*/

bool parcours_escape(Triangulation& t) {

    // read person to escape
    int x, y;
    long d;
    cin >> x >> y >> d;

    // prepare tools for sort of BFS
    unordered_map<Face, bool> visited;
    Face f = t.locate(P(x, y));
    queue<Face> q;
    q.push(f);

    // check if already contaminated
    P p = P(x, y);
    P v0 = (*f).vertex(0)->point();
    P v1 = (*f).vertex(1)->point();
    P v2 = (*f).vertex(2)->point();
    P nearest = t.nearest_vertex(p)->point();

    if (CGAL::squared_distance(p, v0) < d || CGAL::squared_distance(p, v1) < d || CGAL::squared_distance(p, v2) < d) {
        return false;
    }

    // !! very tricky !! if in infinite face, possible that far enough from 3 vertices,
    // but too close to another nearest vertex
    if ((CGAL::squared_distance(p, nearest) < d)) {
        return false;
    }

    if (t.is_infinite(f)) {
        return true;
    }

    // run BFSish with faces insted of vertices. edge if gate large enough.
    while (!q.empty()) {

        // extract face at front
        Face f = q.front();
        // P p = t.dual(f);  
        visited[f] = true;
        q.pop();

        P v0 = (*f).vertex(0)->point();
        P v1 = (*f).vertex(1)->point();
        P v2 = (*f).vertex(2)->point();

        // check if already contaminated. NOT NECESSARY
        // if (CGAL::squared_distance(p, v0) < d || CGAL::squared_distance(p, v1) < d || CGAL::squared_distance(p, v2) < d) {
        //     return false;
        // }

        // check if face infinite, means escape ok
        if (t.is_infinite(f)) {
            return true;
        }

        // push accessible faces not yet visited
        if (CGAL::squared_distance(v0, v1) >= 4*d && visited.count((*f).neighbor(2)) == 0) {
            q.push((*f).neighbor(2));
        }
        if (CGAL::squared_distance(v1, v2) >= 4*d && visited.count((*f).neighbor(0)) == 0) {
            q.push((*f).neighbor(0));
        };
        if (CGAL::squared_distance(v0, v2) >= 4*d && visited.count((*f).neighbor(1)) == 0) {
            q.push((*f).neighbor(1));
        }
    }
    return false;
}

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

    // read query points
    int m;
    cin >> m;

    //try to escape for each query person
    for (int i=0; i<m; i++) {
        bool escape = parcours_escape(t);
        if (escape) cout << "y";
        else cout << "n";
    }
    cout << "\n";
    //cout << n << " " << m << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while (n!=0) {
        testcase(n);
        cin >> n;
    }
}
