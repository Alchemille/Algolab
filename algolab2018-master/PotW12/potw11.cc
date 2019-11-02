#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Warehouse{
	P location;
	int s, a;
};

struct Stadium{
	P location;
	int d, u;
};

struct Circle{
	P location;
	int radius;
};

bool is_contained(Circle &C, Stadium &s){
	return CGAL::squared_distance(C.location, s.location) <= std::pow(C.radius, 2);
}

bool is_contained(Circle &C, Warehouse &w){
	return CGAL::squared_distance(C.location, w.location) <= std::pow(C.radius, 2);
}

void testcase(){
	int n, m , c; std::cin >> n >> m >> c;
	std::vector<Warehouse> warehouses(n);
	std::vector<Stadium> stadiums(m);
	Program lp(CGAL::SMALLER, true, 0, false, 0);
	
	for(int i=0; i<n; i++){
		Warehouse w;
		std::cin >> w.location >> w.s >> w.a;
		warehouses[i] = w;
		lp.set_b(2*m+i, w.s);
	}
	for(int j=0; j<m; j++){
		Stadium s;
		std::cin >> s.location >> s.d >> s.u;
		stadiums[j] = s;
		lp.set_b(j, -s.d);
		lp.set_r(j, CGAL::EQUAL);
		lp.set_b(m+j, s.u * 100);
	}
	std::vector<std::vector<int>> rewards(n, std::vector<int>(m, 0));
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			int reward; std::cin >> reward;
			rewards[i][j] = reward;
			lp.set_a(i*m+j, j, -1); // demand constraint
			lp.set_a(i*m+j, m+j, warehouses[i].a); // alcohol upperbound constraint
			lp.set_a(i*m+j, 2*m+i, 1); //warehouses supply constraint
		}
	}
	std::vector<Circle> contour_lines(c);
	std::vector<std::vector<int>> warehouses_contained(n);
	std::vector<std::vector<int>> stadium_contained(m);
	for(int i=0; i<c; i++){
		std::cin >> contour_lines[i].location >> contour_lines[i].radius;
		for(int j=0; j<warehouses.size(); j++){
			if(is_contained(contour_lines[i], warehouses[j])){
				warehouses_contained[j].push_back(i);
			}
		}
		for(int j=0; j<stadiums.size(); j++){
			if(is_contained(contour_lines[i], stadiums[j])){
				stadium_contained[j].push_back(i);
			}
		}
	}
	// Inclusion exclusion for each pair of warehouse/stadium
	for(int i=0; i<warehouses.size(); i++){
		for(int j=0; j<stadiums.size(); j++){
			int union_number = warehouses_contained[i].size() + stadium_contained[j].size();
			for(int idx: warehouses_contained[i]){
				if(is_contained(contour_lines[idx], stadiums[j])){
					union_number -= 2;
				}
			}
			lp.set_c(i*m+j, -rewards[i][j]*100 + union_number);
		}
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if(sol.is_infeasible()){
		std::cout << "RIOT!";	
	}else{
		std::cout << 0.0 + std::floor(-CGAL::to_double(sol.objective_value()) / 100.0);
	}
	std::cout << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(20);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
