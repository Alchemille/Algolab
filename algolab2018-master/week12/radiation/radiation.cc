#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/MP_Float.h>
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Point{
	int x, y, z;
	Point(int x, int y, int z) : x(x), y(y), z(z) {}
};

bool solve_lp(const int d, const std::vector<Point> &h_pts, const std::vector<Point> &t_pts){
	Program lp(CGAL::SMALLER, false, 0, false, 0);
	int var_nb = 1; 
	for(int x_d=0; x_d<=d; x_d++){
		for(int y_d=0; y_d<=d-x_d; y_d++){
			for(int z_d=0; z_d<=d-x_d-y_d; z_d++){
				// add h_pts constaints
				for(int i=0; i<h_pts.size(); i++){
					Point p = h_pts[i];
					lp.set_a(var_nb, i, std::pow(p.x, x_d) * std::pow(p.y, y_d) * std::pow(p.z, z_d));
				}
				for(int i=0; i<t_pts.size(); i++){
					Point p = t_pts[i];
					lp.set_a(var_nb, i+h_pts.size(), -std::pow(p.x, x_d) * std::pow(p.y, y_d) * std::pow(p.z, z_d));
				}
				var_nb++;
			}
		}
	}
	// set the epsilon variable
	for(int i=0; i<h_pts.size(); i++)
		lp.set_a(0, i, 1);
	for(int i=0; i<t_pts.size(); i++)
		lp.set_a(0, i+h_pts.size(), 1);
	//minimize epsilon
	lp.set_c(0, -1);
	lp.set_l(0, true, 0);
	lp.set_u(0, true, 1);
	
	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution sol = CGAL::solve_linear_program(lp, ET(), options);

	if(sol.is_infeasible() || sol.objective_value() == 0){
		return false;
	}else{
		return true;
	}
}

void testcase(){
	int h, t; std::cin >> h >> t;
	std::vector<Point> h_pts, t_pts;
	for(int i=0; i<h; i++){
		int x, y, z; std::cin >> x >> y >> z;
		h_pts.push_back(Point(x, y, z));
	}
	for(int i=0; i<t; i++){
		int x, y, z; std::cin >> x >> y >> z;
		t_pts.push_back(Point(x, y, z));
	}
	// exponential increases
	int min = 0; int max = 1;
	while(max <= 30 && !solve_lp(max, h_pts, t_pts)){
		max *= 2;
	}
	if(max > 30 && !solve_lp(30, h_pts, t_pts)){
		std::cout << "Impossible!\n";
		return;
	}

	// binary search on d
	while(min != max){
		int mid = (max + min) / 2;
		if(solve_lp(mid, h_pts, t_pts)){
			max = mid;
		}else{
			min = mid + 1;
		}
	}
	std::cout << min << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
