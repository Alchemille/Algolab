#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(SolT s){
	double a = std::floor(CGAL::to_double(s));
	while(a > s) a--;
	while(a + 1 < s) a++;
	return a;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int N, M; std::cin >> N >> M;
	while(N > 0){
		std::vector<int> min_max(N);
		Program lp(CGAL::SMALLER, false, 0, false, 0);
		for(int i=0; i<N; i++){
			int min, max;
			std::cin >> min >> max;
			lp.set_b(i, -min);
			lp.set_b(i+N, max);
		}

		for(int i=0; i<M; i++){
			int p_i; std::cin >> p_i;
			lp.set_c(i, p_i);
			for(int j=0; j<N; j++){
				int n_i_j; std::cin >> n_i_j;
				lp.set_a(i, j, -n_i_j);
				lp.set_a(i, j+N, n_i_j);
			}
		}

		Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());

		if(s.is_infeasible()){
			std::cout << "No such diet.\n";
		}else{
			std::cout << floor_to_double(s.objective_value()) << "\n";
		}

		std::cin >> N >> M;
	}
	return 0;
}

