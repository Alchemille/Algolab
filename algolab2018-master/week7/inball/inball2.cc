#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(){
	std::ios_base::sync_with_stdio(false);
	int N, D;
	while(std::cin >> N){
		std::cin >> D;
		Program lp(CGAL::SMALLER, false, 0, false, 0);
		for(int i=0; i<N; i++){
			uint64_t sum = 0;
			for(int j=0; j<D; j++){
				int64_t coef; std::cin >> coef;
				lp.set_a(i, j, coef);
				sum += coef*coef;
			}
			int64_t b; std::cin >> b;
			lp.set_b(i, b);
			lp.set_a(i, D, std::sqrt(sum));
		}
		lp.set_c(D, 1);
		
		Solution sol = CGAL::solve_linear_program(lp, ET());
		if(sol.is_infeasible()){
			std::cout << "none\n";
		}else if(sol.is_unbounded()){
			std::cout << "inf\n";
		}else{
			std::cout << sol.objective_value() << "\n";
		}
	}
	return 0;
	}
