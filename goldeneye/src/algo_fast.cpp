#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <unordered_map>
#include <queue>
#include <assert.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex. Edges not represented explicitly.
// Represent them as (index1, index2, distance)
typedef std::size_t                                            Index; // unsigned interger type
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2                                             P;
typedef std::tuple<Index,Index,K::FT>                          Edge;
typedef Delaunay::Vertex_handle                                Vertex;

/*
tricks / traps:
- K::FT a, b and not int a, b. Otherwise commparison with edges distance wont work
- Weird flags
- regex to find first line of tests ^\d+ \d+ \d+$
*/ 

using namespace std;

void testcase() {

    long n, m;
    double p;
    cin >> n >> m >> p;

    // Read and index jammers
    std::vector<std::pair<P,Index>> jammers(n);
    for (Index i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        jammers.emplace_back(P(x, y), i);
    }

    // read missions
    vector<pair<P, P>> missions;

    for (int i=0; i<m; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        missions.push_back({P(x1, y1), P(x2, y2)});
    }

    // build triangulation and store edges
    Delaunay t;
    t.insert(jammers.begin(), jammers.end());

    vector<Edge> edges;

    for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); ++it) {
        int i1 = (*it).first->vertex(((*it).second + 1) % 3)->info();
        int i2 = (*it).first->vertex(((*it).second + 2) % 3)->info();
        if (i1 > i2) std::swap(i1, i2);
        edges.push_back({i1, i2, t.segment(it).squared_length()});
    }

    // sort edges and compute mst
    std::sort(edges.begin(), edges.end(),
	    [](const Edge& e1, const Edge& e2) -> bool { // kinda lambda function
	      return std::get<2>(e1) < std::get<2>(e2); // sort by length
            });


    // compute components with power consumption p
    boost::disjoint_sets_with_storage<> ufp(n);

    for (vector<Edge>::iterator e = edges.begin(); e != edges.end() && std::get<2>(*e) <= p; ++e) {
        ufp.union_set(std::get<0>(*e), std::get<1>(*e));
    }

    // // union find for p coverage. Not a MST
    // boost::disjoint_sets_with_storage<> ufp(n);
    Index n_components = n;

    // answer first question: for each mission, check if s and t are in same CC of ufp
    vector<K::FT> distances_st(m); // save max distance of (s, nearest) and (t, nearest) for each jammer
    vector<bool> result_qu1(m, false);
    for (int i = 0; i < m; i ++) {

        P s = missions[i].first;
        P e = missions[i].second;

        K::FT d1 = CGAL::squared_distance(s, t.nearest_vertex(s)->point()) * 4;
        K::FT d2 = CGAL::squared_distance(e, t.nearest_vertex(e)->point()) * 4;
        K::FT d = max(d1, d2);
        distances_st[i] = d;

        int v1 = t.nearest_vertex(s)->info();
        int v2 = t.nearest_vertex(e)->info();

        Index is = ufp.find_set(v1);
        Index ie = ufp.find_set(v2);

        if (is == ie && d <= p) {
            cout << "y";
            result_qu1[i] = true;
        }
        else {
            cout << "n";
        }
    }
    cout << "\n";

    // save interesting w values to answer qu2 and qu3. MST Kruskal consists in joining 2 CC when adding an edge.
    // MST edges are the interesting w values.

    boost::disjoint_sets_with_storage<> uf_mst(n);
    n_components = n;
    vector<Edge> mst_w(n); // list for the mst. Edges are sorted since sorting previously
    
    for (vector<Edge>::iterator e = edges.begin(); e != edges.end(); ++e) {

        // find representants of each edge endpoint
        Index c1 = uf_mst.find_set(std::get<0>(*e));
        Index c2 = uf_mst.find_set(std::get<1>(*e));

        if (c1 != c2) {

            // this edge connects two different components => part of the emst
            uf_mst.link(c1, c2);
            mst_w.push_back(*e);
            if (--n_components == 1) break; // all the edges are linked
        }
    }

    // iterate over mst_w and missions to answer to qu2 and qu3: for each mst_w, test if can accomplish the mission
    K::FT a = 0;
    K::FT b = 0;

    // answer qu2: add mst edges from small to big until s and t in same CC
    boost::disjoint_sets_with_storage<> ufa(n); // initialize ufa
    boost::disjoint_sets_with_storage<> ufb(n); // initialize ufb

    vector<Edge>::iterator ea = mst_w.begin();
    vector<Edge>::iterator eb = mst_w.begin();

    for (int i = 0; i < m; i ++) {

        P s = missions[i].first;
        P e = missions[i].second;

        int v1 = t.nearest_vertex(s)->info();
        int v2 = t.nearest_vertex(e)->info();

        a = max(a, distances_st[i]);
        if (result_qu1[i] == 1) b = max(b, distances_st[i]);

        for (; ea != mst_w.end(); ++ ea) {
            Index is = ufa.find_set(v1); // CC for s
            Index ie = ufa.find_set(v2); // CC for t
            if (is == ie) {
                break;
            }
            Index c1 = ufa.find_set(std::get<0>(*ea));
            Index c2 = ufa.find_set(std::get<1>(*ea));
            if (c1 != c2) {
                a = max(a, std::get<2>(*ea));
                ufa.link(c1, c2);
            }
            
        }
        if (result_qu1[i] == 1) {
            for (; eb != mst_w.end(); ++ eb) {
                Index is = ufb.find_set(v1); // CC for s
                Index ie = ufb.find_set(v2); // CC for t
                if (is == ie) {
                    break;
                }
                Index c1 = ufb.find_set(std::get<0>(*eb));
                Index c2 = ufb.find_set(std::get<1>(*eb));
                if (c1 != c2) {
                    b = max(b, std::get<2>(*eb));
                    ufb.link(c1, c2);
                }
            }
        }
    }

    cout << a << "\n" << b << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);    
    int t;
    cin >> t;
    while (t--) testcase();
}
