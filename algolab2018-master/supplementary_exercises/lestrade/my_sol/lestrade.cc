#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

struct L{
	int u, v, w, id;
	L(){u=0; v=0; w=0;id=0;}
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<L, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Tr;
typedef K::Point_2 P;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(){
	int z, u, v, w; std::cin >> z >> u >> v >> w;
	int a, g; std::cin >> a >> g;
	std::vector<std::pair<P, L>> gang(g);
	std::vector<P> agents(a);
	std::vector<int> wages(a);

	for(int i=0; i<g; i++){
		std::cin >> gang[i].first;
		std::cin >> gang[i].second.u;
		std::cin >> gang[i].second.v;
		std::cin >> gang[i].second.w;
		gang[i].second.id = i;
	}
	Tr tr; tr.insert(gang.begin(), gang.end());
	for(int i=0; i<a; i++){
		std::cin >> agents[i];
		std::cin >> wages[i];
	}
	
	// Construct linear program
	Program lp(CGAL::SMALLER, true, 0, true, 24);
	enum {WHERE, WHO, HOW};
	std::vector<int> used_gang(g, -1);
	for(int i=0; i<a; i++){
		L closest_agent = tr.nearest_vertex(agents[i])->info();
		if(used_gang[closest_agent.id] >= 0){
			if(wages[i] < wages[used_gang[closest_agent.id]])
				used_gang[closest_agent.id] = i;
		}else{
			used_gang[closest_agent.id] = i;
		}
	}

	for(int i=0; i<g; i++){
		if(used_gang[i] >= 0){
			lp.set_a(used_gang[i], WHERE, -gang[i].second.u);
			lp.set_a(used_gang[i], WHO, -gang[i].second.v);
			lp.set_a(used_gang[i], HOW, -gang[i].second.w);
			lp.set_c(used_gang[i], wages[used_gang[i]]);
		}
	}
	lp.set_b(WHERE, -u);
	lp.set_b(WHO, -v);
	lp.set_b(HOW, -w);

	Solution s = CGAL::solve_linear_program(lp, ET());

	if(s.is_infeasible() || s.objective_value() > z)
		std::cout << "H\n";
	else
		std::cout << "L\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
