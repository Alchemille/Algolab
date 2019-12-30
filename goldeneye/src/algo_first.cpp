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
First, compute Delaunay triangulaton and the mst.
Then, for each mission, compute path from start to end of mission.
Keep track of biggest edge on this path.
Array of biggest edges allow to answer 3 questions.
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

    // implement Kruskal with union-find. Record mst with adjacency list
    boost::disjoint_sets_with_storage<> uf(n);
    Index n_components = n;
    vector<vector<pair<int, K::FT>>> adj_list(n); // adjacency list for the mst
    
    for (vector<Edge>::iterator e = edges.begin(); e != edges.end(); ++e) {

        // find representants of each edge endpoint
        Index c1 = uf.find_set(std::get<0>(*e));
        Index c2 = uf.find_set(std::get<1>(*e));

        if (c1 != c2) {

            // this edge connects two different components => part of the emst
            uf.link(c1, c2);
            adj_list[c1].push_back({c2, std::get<2>(*e)});
            adj_list[c2].push_back({c1, std::get<2>(*e)});

            if (--n_components == 1) break; // all the edges are linked
        }
    }

    // for each mission, find closest Delaunay vertices to s and t, then part of mst to traverse
    vector<K::FT> max_edges(m);
    int i = 0;
    for (auto it = missions.begin(); it != missions.end(); ++it) {

        P s = (*it).first; // start point   
        P e = (*it).second; // end point

        int v1 = t.nearest_vertex(s)->info();
        int v2 = t.nearest_vertex(e)->info();

        K::FT d1 = CGAL::squared_distance(s, t.nearest_vertex(s)->point()) * 4;
        K::FT d2 = CGAL::squared_distance(e, t.nearest_vertex(e)->point()) * 4;
        K::FT d = max(d1, d2);

        if (v1 != v2) {

            // find mst path from v1 to v2, with simple BFS
            queue<int> Q;
            vector<bool> visited(n, false);
            unordered_map<int, pair<int, K::FT>> predecessor;// (predecessor, distance)
            Q.push(v1);
            visited[v1] = true;


            while(!Q.empty()) {
                int u = Q.front();
                Q.pop();

                // push and visit unvisited neighbors
                for (auto it = adj_list[u].begin(); it != adj_list[u].end(); ++it) {
                    
                    int v = (*it).first;
                    int distance_uv = (*it).second;
                    
                    if (!visited[v]) {

                        Q.push(v);
                        predecessor[v] = {u, distance_uv};
                        visited[v] = true;
                        
                        if (v == v2) break;
                    }
                }
            }

            // find longest edge on the path found in the mst
            int pred = predecessor[v2].first;
            d = max(d, predecessor[v2].second);

            while (pred != v1) {
                pred = predecessor[pred].first;
                d = max(d, predecessor[pred].second);
            }
        }

        // record max_edge for this mission
        max_edges[i] = d;
        i++;

        cerr << d << " ";
    }
    assert (i == missions.size());
    cerr << p << "\n";

    // qu.1
    K::FT result_qu2 = 0;
    K::FT result_qu3 = 0;

    for (auto it = max_edges.begin(); it != max_edges.end(); ++it) {
        if ((*it) <= p) {
            std::cout << "y";
            result_qu3 = max(result_qu3, (*it));
        }
        else {std::cout << "n";}

        result_qu2 = max(result_qu2, *it);
    }
    std::cout << "\n";
    std::cout << result_qu2 << "\n";
    std::cout << result_qu3 << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
