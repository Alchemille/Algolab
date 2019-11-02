#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef	CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb; 
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Tr;
typedef K::Point_2 P;

double rad_to_time(double r){
	return std::sqrt(r - 0.5);
}

void testcase(){
	int n; std::cin >> n;
	int l, b, r, t; std::cin >> l >> b >> r >> t;
	std::vector<std::pair<P, int>> bacteries(n);
	for(int i=0; i<n; i++){
		P p; std::cin >> p;
		bacteries[i] = std::make_pair(p, i);
	}
	Tr tr; tr.insert(bacteries.begin(), bacteries.end());
	
	std::vector<double> min_distances(n, std::numeric_limits<double>::max());
	for(auto e = tr.finite_edges_begin(); e != tr.finite_edges_end(); e++){
		Tr::Vertex_handle v1 = e->first->vertex((e->second+1)%3);	
		Tr::Vertex_handle v2 = e->first->vertex((e->second+2)%3);	
		min_distances[v1->info()] = CGAL::min(min_distances[v1->info()],
			tr.segment(e).squared_length());
		min_distances[v2->info()] = CGAL::min(min_distances[v2->info()],
			tr.segment(e).squared_length());
	}

	// Apply the box constraints
	for(int i=0; i<n; i++){
		min_distances[i] = CGAL::min(min_distances[i], (l - bacteries[i].first.x())*(l - bacteries[i].first.x()));
		min_distances[i] = CGAL::min(min_distances[i], (r - bacteries[i].first.x())*(r - bacteries[i].first.x()));
		min_distances[i] = CGAL::min(min_distances[i], (t - bacteries[i].first.y())*(t - bacteries[i].first.y()));
		min_distances[i] = CGAL::min(min_distances[i], (b - bacteries[i].first.y())*(b - bacteries[i].first.y()));
	}
	
	std::sort(min_distances.begin(), min_distances.end());
	// Output the results
	std::cout << rad_to_time(min_distances[0])
		<< " " << rad_to_time(min_distances[(min_distances.size()-1)/2])
			<< " " << rad_to_time(min_distances[min_distances.size()-1]) << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
