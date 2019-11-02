#include <bits/stdc++.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct P{
	int64_t x, y, z;
	P(int64_t x, int64_t y, int64_t z) : x(x), y(y), z(z) {}
	P(){}
};

bool separable(std::vector<P> &healthy, std::vector<P> &tumourous, const int64_t d){
	Program lp(CGAL::SMALLER, false, 0, false, 0);
	// healthy constraints
	int64_t delta;
	for(int i=0; i<healthy.size(); i++){
		int64_t var_id = 0;
		for(int64_t x=0; x<=d; x++){
			for(int64_t y=0; y<=d-x; y++){
				for(int64_t z=0; z<=d-x-y; z++){
					P p = healthy[i];
					int64_t coef = std::pow(p.x, x) * std::pow(p.y, y) * std::pow(p.z, z);
					lp.set_a(var_id, i, coef);
					var_id++;
				}
			}
		}
		lp.set_a(var_id, i, -1);
		lp.set_r(i, CGAL::LARGER);
		delta = var_id;
	}
	int64_t offset = healthy.size();
	for(int i=0; i<tumourous.size(); i++){
		int64_t var_id = 0;
		for(int64_t x=0; x<=d; x++){
			for(int64_t y=0; y<=d-x; y++){
				for(int64_t z=0; z<=d-x-y; z++){
					P p = tumourous[i];
					int64_t coef = std::pow(p.x, x) * std::pow(p.y, y) * std::pow(p.z, z);
					lp.set_a(var_id, i+offset, coef);
					var_id++;
				}
			}
		}
		lp.set_a(var_id, i+offset, 1);
		lp.set_r(i+offset, CGAL::SMALLER);
		assert(var_id = delta);
	}
	
	lp.set_l(delta, true, 0);
	lp.set_u(delta, true, 1);
	lp.set_c(delta, -1);
//	CGAL::print_linear_program(std::cerr, lp);
	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution sol = CGAL::solve_linear_program(lp, ET(), options);
	if(sol.is_infeasible() || sol.objective_value() == 0)
		return false;
	else
		return true;
}

void testcase(){
	uint64_t h, t; std::cin >> h >> t;
	std::vector<P> healthy(h), tumourous(t);	
	for(int i=0; i<h; i++){
		int64_t x, y, z; std::cin >> x >> y >> z;
		healthy[i] = P(x, y, z);
	}
	for(int i=0; i<t; i++){
		int64_t x, y, z; std::cin >> x >> y >> z;
		tumourous[i] = P(x, y, z);
	}

	// Exponential increments search
	int64_t min = 1, max = 1;
	while(!separable(healthy, tumourous, max)){
		if(max >= 30){
			std::cout << "Impossible!\n";
			return;
		}
		min = max;
		max *= 2; max = std::min((int64_t)30, max);
	}
	// Binary search
	max++;
	int i=0;
	while(min != max){
		int64_t m = (min+max)/2;
		if(separable(healthy, tumourous, m)){
			max = m;
		}else{
			min = m+1;
		}
	}
	std::cout << min << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
