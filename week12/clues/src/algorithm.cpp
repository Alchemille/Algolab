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
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 P;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef Delaunay::Vertex_handle vh;

/*
Correction: 
Interesting use of info for triangulation. <pair<int, bool>, K> instead of <Index, K>
info is the part before K, accessible with vertex_handle->info()

blocages:
- long for distance
- did not understand problem (closest stations only from a or to b)

first question:
at first, wanted to do 2coloring on graph G (edge if distance ok), but complexity too large (can be V^2 edges)
Doing it over triangulation after removing excessive edges, or over MST after removing big edges, is enough.
IF we do sanity check afterwards.
Consist in recording in 2 other triangulations the vertices of each color,
and check the minimal distance between 2 vertices of same color is large enough

second question:
at first, wanted to add a and b to the CCs. Did not consider that signal goes to the closest station.
*/

using namespace std;


bool DFSvisit(vector<vector<int>>& adj_list, int u, int color, vector<int>& colors, 
    vector<std::pair<P,Index>>& colors1, vector<std::pair<P,Index>>& colors2, vector<std::pair<P,Index>>& stations) {
    
    colors[u] = color;
    
    for(int v : adj_list[u]) {

        if (colors[v] == 0) {
            bool possible;
            if (color == 1) {
                colors2.push_back(stations[v]);
                possible = DFSvisit(adj_list, v, 2, colors, colors1, colors2, stations);
            }
            else {
                colors1.push_back(stations[v]);
                possible = DFSvisit(adj_list, v, 1, colors, colors1, colors2, stations);
            }
            if (!possible) return false;
        }
    }
    return true;
}

bool sanity(Delaunay& triang, int r) {
    for (auto it = triang.finite_edges_begin(); it != triang.finite_edges_end(); ++it) {
        if (triang.segment(it).squared_length() <= r * r) {
            return false;
        }
    }
    return true;            
}

void testcase() {

    Index n, m, r;
    cin >> n >> m >> r;

    // create Delaunay
    vector<std::pair<P,Index>> stations;
    stations.reserve(n);
    for (int i = 0; i < n; i ++) {
        int x, y;
        cin >> x >> y;
        stations.emplace_back(P(x, y), i);
    }
    Delaunay t;
    t.insert(stations.begin(), stations.end());


    // read missions
    vector<std::pair<P, P>> missions;
    missions.reserve(m);
    for (int i = 0; i < m; i ++) {
        int ax, ay, bx, by;
        cin >> ax >> ay >> bx >> by;
        missions.emplace_back(P(ax, ay), P(bx, by));
    }

    // compute MST (extract sort edges first)
    EdgeV edges;
    edges.reserve(3*n); // there can be no more in a planar graph

    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        // ensure smaller index comes first
        if (i1 > i2) std::swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(),
            [](const Edge& e1, const Edge& e2) -> bool {
            return std::get<2>(e1) < std::get<2>(e2);
                });


    // UF to compute MST and save MST edges
    boost::disjoint_sets_with_storage<> uf_mst(n);
    vector<Edge> mst_edges(n - 1);
    Index n_components = n;

    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
        // edges of trigangulation so O(v) still

        // determine components of endpoints
        Index c1 = uf_mst.find_set(std::get<0>(*e));
        Index c2 = uf_mst.find_set(std::get<1>(*e));

        if (c1 != c2) {
            mst_edges.push_back(*e);
            uf_mst.link(c1, c2);
            if (--n_components == 1) break;
        }
    }

    // compute range-graph (CC) over MST. Amouts to remove edges bigger than r*r. Or could compute other UF to get exact CCs.
    vector<Edge> range_edges(n - 1);
    for (auto it = mst_edges.begin(); it != mst_edges.end(); ++ it) {
        if (std::get<2>(*it) <= r * r) {
            range_edges.push_back(*it);
        }
    }

    // create adj list for DFS
    vector<vector<int>> adj_list(n, vector<int>());
    for (auto it = mst_edges.begin(); it != mst_edges.end(); ++it) {
        int u = std::get<0>(*it);
        int v = std::get<1>(*it);
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }

    // 2-coloring of each range-CC with DFS
    vector<int> colors(n, 0); // 3 possible colors: unvisited 0, color1_triang, color2_triang
    vector<std::pair<P,Index>> colors1; // record stations of each color for sanity check
    vector<std::pair<P,Index>> colors2;

    bool possible = true;

    for(int i = 0; i < n; i ++) { // make sure to visit all the trees

        if (colors[i] == 0) {

            possible = DFSvisit(adj_list, i, 1, colors, colors1, colors2, stations); // returns whether 2coloring possible
            colors1.push_back(stations[i]);
            if (!possible) break;
        }
    }

    // check if 2coloring of each CC works, by creating triangulation for each color and each station
    // ok to do it over all CCs, no need to loop over CCs
    Delaunay color1_triang;
    Delaunay color2_triang;
    color1_triang.insert(colors1.begin(), colors1.end());
    color2_triang.insert(colors2.begin(), colors2.end());

    bool check_1 = sanity(color1_triang, r); // in O(V) since triangulatiobn
    bool check_2 = sanity(color2_triang, r);

    // check if 2coloring of mst worked
    if (!possible || !check_1 || !check_2) {
        for (int i = 0; i < m; i ++) {
            cout << "n";
        }
        cout << "\n";
        return;
    }
    
    // answer second question if 2 coloring doable. COmpute CC explicitly with UF (not just MST edges of the CC)
    // we could also have used the MST CCs to fill the UF, to examine less edge!
    boost::disjoint_sets_with_storage<> uf_range(n);

    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {

        // determine components of endpoints
        Index c1 = uf_range.find_set(std::get<0>(*e));
        Index c2 = uf_range.find_set(std::get<1>(*e));

        if (c1 != c2 && CGAL::squared_distance(stations[std::get<0>(*e)].first, stations[std::get<1>(*e)].first) <= r * r) {
            uf_range.link(c1, c2);
        }
    }

    for (auto paire : missions) {

        P sherlock = paire.first;
        P watson = paire.second;
        vh V_sherlock = t.nearest_vertex(sherlock);
        vh V_watson = t.nearest_vertex(watson);
        int cSherlock = uf_range.find_set(V_sherlock->info());
        int cWatson = uf_range.find_set(V_watson->info());

        if (CGAL::squared_distance(sherlock, watson) <= r * r) {
            cout << "y";
        }
        else if (cWatson == cSherlock && 
            CGAL::squared_distance(sherlock, V_sherlock->point()) <= r * r &&
            CGAL::squared_distance(watson, V_watson->point()) <= r * r) {   
            cout << "y";
        }
        else {
            cout << "n";
        }
    }
    cout << "\n";
}


int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}