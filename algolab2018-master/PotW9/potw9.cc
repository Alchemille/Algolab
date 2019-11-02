#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Face_handle F_h;
typedef Triangulation::Vertex_handle V_h;

typedef boost::disjoint_sets_with_storage<> Uf;

struct Edge{
	Edge(int u, int v, K::FT length) : u(u), v(v), length(length) {}
	int u, v;
	K::FT length;
	bool operator<(const Edge d2) const{
		return length < d2.length;
	}
};

void testcase(){
	int n, m; long p;
	std::cin >> n >> m >> p;
	Triangulation tr;
	std::vector<std::pair<K::Point_2, int>> pts;
	for(int i=0; i<n; i++){
		K::Point_2 p; std::cin >> p;
		pts.push_back(std::make_pair(p, i));
	}
	tr.insert(pts.begin(), pts.end());
	
	// build connected components of energy p
	// Iterate over finite edges of the triangulation and compute connected components
	Uf union_find_p(n); 
	std::vector<Edge> edges;
	edges.reserve(3*n);
	for(auto itr = tr.finite_edges_begin(); itr != tr.finite_edges_end(); itr++){
		int u = itr->first->vertex((itr->second + 1) %3)->info();
		int v = itr->first->vertex((itr->second + 2) %3)->info();
		edges.push_back(Edge(u, v, CGAL::squared_distance(itr->first->vertex((itr->second + 1) %3)->point(), itr->first->vertex((itr->second + 2) %3)->point())));
	}
	std::sort(edges.begin(), edges.end());
	for(Edge e: edges){
		if(e.length <= p){
			union_find_p.union_set(e.u, e.v);	
		}
	}
	K::FT a = 0;
	Uf union_a(n);
	auto edge_it_a = edges.begin();
	K::FT b = 0;
	Uf union_b(n);
	auto edge_it_b = edges.begin();

	for(int i=0; i<m; i++){
		K::Point_2 s, t;
		std::cin >> s >> t;
	
		V_h curr = tr.nearest_vertex(s);
		V_h target = tr.nearest_vertex(t);

		K::FT max_d = std::max(4*CGAL::squared_distance(curr->point(), s), 4*CGAL::squared_distance(target->point(), t));

		if(max_d <= p
			&& union_find_p.find_set(curr->info()) == union_find_p.find_set(target->info())){
			std::cout << "y";
			// Doable with power p: must be doable with power b
			 auto edge_it = edges.begin();
			 while(edge_it_b != edges.end() && union_b.find_set(curr->info()) != union_b.find_set(target->info())){
				 auto e = *edge_it_b;
				 if(b < e.length)
					b = e.length;
				 union_b.union_set(e.u, e.v);
				 edge_it_b++;
			 }
			 if(max_d > b)
				 b = max_d;
		}else{
			std::cout << "n";
		}
		 // Compute a using the Pim's MST technique
		 while(edge_it_a != edges.end() && union_a.find_set(curr->info()) != union_a.find_set(target->info())){
			 auto e = *edge_it_a;
			 if(a < e.length)
			 	a = e.length;
			 union_a.union_set(e.u, e.v);
			 edge_it_a++;
		 }
		 if(max_d > a)
			 a = max_d;
	}
	if (edge_it_a != edges.begin() && (edge_it_a - 1)->length > a)
		a = (edge_it_a-1)->length;
	std::cout << "\n" <<  a << "\n";
	std::cout << b;
	std::cout << std::endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(20);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		testcase();
	}
	return 0;
}
