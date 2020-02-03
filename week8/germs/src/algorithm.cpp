#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <unordered_map>

typedef std::size_t                  Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;

typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> vertex_t;
typedef CGAL::Triangulation_face_base_2<K> face_t;

typedef CGAL::Triangulation_data_structure_2<vertex_t,face_t> triangulation_t;
typedef CGAL::Delaunay_triangulation_2<K, triangulation_t> Triangulation;

typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 P;
typedef K::Segment_2 S;

using namespace std;

/*
For each bacteria, compute distance to closest bacteria and distance to walls
One have dying time of every bacterium, can sort and compute outputs.
2 ways to compute distance to closest bactiria:
- iterate over finite_edges()
- iterate over finite_vertices()

We use info to index the vertices and store distance for each one.
In correction, they actually store the dying_distance in info!
IK::FT & info_g = v->info();
Careful: Kernel with sqrt is super slow !
Actually, there was not even need for Kernel_sqrt
*/

int distance_to_time(K::FT const & d) {
    double t_square = (std::sqrt(d) - 1) / 2;
    double t = std::sqrt(t_square);
    int result = ceil(CGAL::to_double(t));
    return result; 
}

void testcase(int n) {

    int l, b, r, t;
    cin >> l >> b >> r >> t;
    vector<pair<P, Index>> pts(n);
    int x, y;    

    S top, bottom, left, right;
    top = S(P(l, t), P(r, t));
    bottom = S(P(l, b), P(r, b));
    left = S(P(l, b), P(l, t));
    right = S(P(r, b), P(r, t));

    for (int i=0; i<n; i++) {
        cin >> x >> y;
        pts[i] = {P(x, y), i};
    }

    // construct triangulation
    Triangulation T;
    T.insert(pts.begin(), pts.end());

    // for each bacteria, compute smallest distance to another bacteria
    vector<K::FT> time_die(n, INT64_MAX);
    for (auto it = T.finite_edges_begin(); it != T.finite_edges_end(); ++ it) {
        
        Vertex v1 = it->first->vertex((it->second + 1) % 3);
        Vertex v2 = it->first->vertex((it->second + 2) % 3);
        K::FT d = CGAL::squared_distance(v1->point(), v2->point());
        
        int i1 = v1->info();
        int i2 = v2->info();
        time_die[i1] = min(time_die[i1], d);
        time_die[i2] = min(time_die[i2], d);
    }

    // for each bacteria and wall, compute distance
    for (auto it = T.finite_vertices_begin(); it != T.finite_vertices_end(); ++ it) {
        
        K::FT d_top = CGAL::squared_distance(it->point(), top);
        K::FT d_bottom = CGAL::squared_distance(it->point(), bottom);
        K::FT d_left = CGAL::squared_distance(it->point(), left);
        K::FT d_right = CGAL::squared_distance(it->point(), right);

        int i = it->info();
        time_die[i] = min(time_die[i], 4 * d_top);
        time_die[i] = min(time_die[i], 4 * d_bottom);
        time_die[i] = min(time_die[i], 4 * d_left);
        time_die[i] = min(time_die[i], 4 * d_right);
    }

    // compute exact times
    sort(time_die.begin(), time_die.end());

    cout << distance_to_time(time_die[0]) << " ";
    cout << distance_to_time(time_die[n / 2]) << " ";
    cout << distance_to_time(time_die[n - 1]) << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}
