#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2 P;
typedef IK::Segment_2 S;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpzf ET;
#include <CGAL/MP_Float.h>
//typedef CGAL::MP_Float ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(){
	int l, s, w; std::cin >> l >> s >> w;
	std::vector<P> lamps(l);
	std::vector<P> stamps(s);
	std::vector<double> max_intensities(s);
	std::vector<S> walls(w);
	
	for(int i=0; i<l; i++)
		std::cin >> lamps[i];
	for(int i=0; i<s; i++){
		std::cin >> stamps[i];
		std::cin >> max_intensities[i];
	}
	for(int i=0; i<w; i++){
		P s, t; std::cin >> s >> t;
		walls[i] = S(s, t);
	}
	
	// Set up the constains
	Program lp(CGAL::SMALLER, true, 1, true, (1 << 12));
	
	for(int j=0; j<s; j++){
		for(int i=0; i<l; i++){
			double rr = CGAL::squared_distance(lamps[i], stamps[j]);
			bool hidden = false;
			for(auto w: walls){
				if(CGAL::do_intersect(S(lamps[i], stamps[j]), w)){
					hidden = true;
				}
			}
			lp.set_a(i, j+s, hidden ? 0 : 1/rr);
			lp.set_a(i, j, hidden ? 0 : 1/rr);
		}
		lp.set_b(j+s, max_intensities[j]);
		lp.set_b(j, 1);
		lp.set_r(j, CGAL::LARGER);
	}
	for(int i=0; i<l; i++)
		lp.set_c(i, 1);

//	CGAL::print_linear_program(std::cerr, lp);	
	Solution sol = CGAL::solve_linear_program(lp, ET());
	
	if(sol.is_infeasible())
		std::cout << "no\n";
	else
		std::cout << "yes\n";

}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
