#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

typedef CGAL::Gmpzf ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(){
	std::ios_base::sync_with_stdio(false);
	int N, M; std::cin >> N >> M;
	while(N>0 && M>0){
		Program qp(CGAL::SMALLER, true, 0, false, 0);
		const int COST_CONSTRAINT = 0;
		const int RETURN_CONSTRAINT = 1;

		for(int i=0; i<N; i++){
			int c_i, r_i;
			std::cin >> c_i >> r_i;
			qp.set_a(i, COST_CONSTRAINT, c_i);
			qp.set_a(i, RETURN_CONSTRAINT, -r_i);
		}

		for(int i=0; i<N; i++){
			for(int j=0; j<N; j++){
				int v_i_j; std::cin >> v_i_j;
				qp.set_d(i, j, 2*v_i_j);
			}
		}

		int C, R, V;
		for(int i=0; i<M; i++){
			std::cin >> C >> R >> V;
			qp.set_b(COST_CONSTRAINT, C);
			qp.set_b(RETURN_CONSTRAINT, -R);

			Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
			
			if(s.is_infeasible()){
				std::cout << "No.\n";
			}else{
				//std::cerr << s.objective_value() << " " << V << "\n";
				if(s.objective_value() <= V){
					std::cout << "Yes.\n";
				}else{
					std::cout << "No.\n";
				}
			}
		}

		std::cin >> N >> M;
	}
	return 0;
}
