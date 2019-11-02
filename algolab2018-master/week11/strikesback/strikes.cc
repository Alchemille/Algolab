#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/utils_classes.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;

struct Particule{
	P point;
	int density;
};

bool reachable(P p1, P p2, K::FT max_radius){
	if(CGAL::squared_distance(p1, p2) > max_radius)
		return false;
	else
		return true;
}

void testcase(){
	int a, s, b; std::cin >> a >> s >> b;
	int e; std::cin >> e;
	std::vector<Particule> particles(a);
	for(int i=0; i<a; i++){
		std::cin >> particles[i].point >> particles[i].density;
	}
	std::vector<P> targets(s);
	for(int i=0; i<s; i++){
		std::cin >> targets[i];	
	}
	std::vector<P> hunters(b);
	for(int i=0; i<b; i++){
		std::cin >> hunters[i];
	}
	Triangulation tr;
	tr.insert(hunters.begin(), hunters.end());
	// compute max radius for each target 
	std::vector<K::FT> max_rad(s, std::numeric_limits<double>::max());
	if(b > 0){
		for(int i=0; i<s; i++){
			Triangulation::Vertex_handle nearest_bh = tr.nearest_vertex(targets[i]);
			max_rad[i] = CGAL::squared_distance(nearest_bh->point(), targets[i]);
		}
	}
	Program lp(CGAL::SMALLER, true, 0, false, 0);
	// add a constraints on s variables
	for(int i=0; i<a; i++){	
		for(int j=0; j<s; j++){
			if(reachable(particles[i].point, targets[j], max_rad[j])){
				lp.set_a(j, i, -1/CGAL::max(1.0, CGAL::squared_distance(particles[i].point, targets[j])));
			}
		}
		lp.set_b(i, -particles[i].density);
	}
	for(int i=0; i<s; i++)
		lp.set_c(i, 1);

	//CGAL::print_linear_program(std::cerr, lp);

  	Solution sol = CGAL::solve_linear_program(lp, ET());

	if(sol.is_infeasible() || sol.is_unbounded() || sol.objective_value() > e)
		std::cout << "n\n";
	else
		std::cout << "y\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
