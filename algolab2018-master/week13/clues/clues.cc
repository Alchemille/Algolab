#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef boost::disjoint_sets_with_storage<> UF;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::pair<int, int> info_t;
typedef CGAL::Triangulation_vertex_base_with_info_2<info_t, K> vertex_t;
typedef CGAL::Triangulation_face_base_2<K> face_t;
typedef CGAL::Triangulation_data_structure_2<vertex_t, face_t> tr_traits;
typedef K::Point_2 P;
typedef CGAL::Delaunay_triangulation_2<K, tr_traits> Tr;


void testcase(){
	int n, m, r; std::cin >> n >> m >> r;
	uint64_t rr = r*r;
	UF uf(n);
	std::vector<std::pair<P, std::pair<int, int>>> stations(n);
	for(int i=0; i<n; i++){
		P p; std::cin >> p;
		stations[i] = std::make_pair(p, std::make_pair(0, 2));
	}
	Tr tr;
	tr.insert(stations.begin(), stations.end());

	// DFS of the delaunay triangulation to create coloring and components
	int components = 0;
	bool doable = true;
	Tr tr0, tr1;
	for(auto v_itr = tr.finite_vertices_begin(); doable && v_itr != tr.finite_vertices_end(); v_itr++){
		// not seen yet if component = 0
		if(v_itr->info().first == 0){
			v_itr->info().first = ++components;
			v_itr->info().second = 0;

			//start a dfs from this face
			std::stack<Tr::Vertex_handle> S;
			S.push(v_itr);

			while(doable && !S.empty()){
				auto v = S.top();

				S.pop();
				auto n = tr.incident_vertices(v);
				do{
					if(!tr.is_infinite(n) && CGAL::squared_distance(v->point(), n->point()) <= std::pow(r, 2)){
						if(n->info().second == v->info().second){
							doable = false;
						}else{
							n->info().second = (v->info().second+1)%2;
							if(n->info().first == 0)
								S.push(n);
						}
						n->info().first = components;
					}
					n++;
				}while(doable && n != tr.incident_vertices(v));
			}
		}
		if(v_itr->info().second == 0)
			tr0.insert(v_itr->point());
		else
			tr1.insert(v_itr->point());
	}

	for(auto e_itr = tr0.finite_edges_begin(); doable && e_itr != tr0.finite_edges_end(); e_itr++){
		if(tr.segment(*e_itr).squared_length() <= std::pow(r, 2)){
			doable = false;
		}
	}
	for(auto e_itr = tr1.finite_edges_begin(); doable && e_itr != tr1.finite_edges_end(); e_itr++){
		if(tr.segment(*e_itr).squared_length() <= std::pow(r, 2)){
			doable = false;
		}
	}

	// check clues
	for(int i=0; i<m; i++){
		P a, b;
		std::cin >> a >> b;

		if(doable){
			// corner case: Watson and Holmes can directly communicate
			if(CGAL::squared_distance(a, b) <= std::pow(r, 2)){
				std::cout << "y";
			}else{
				auto n_a = tr.nearest_vertex(a);
				auto n_b = tr.nearest_vertex(b);
				if(n_a->info().first == n_b->info().first && CGAL::squared_distance(a, n_a->point()) <= std::pow(r, 2)
					&& CGAL::squared_distance(b, n_b->point()) <= std::pow(r, 2)){
					std::cout << "y";
				}else{
					std::cout << "n";
				}
			}
		}else{
			std::cout << "n";
		}
	}
	std::cout << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
