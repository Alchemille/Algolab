#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(SolT e){
	double d = std::floor(CGAL::to_double(e));
	while(d > e) d--;
	while (d+1 < e) d++;
	return d;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	while(N != 0){
		int d; std::cin >> d;
		const int R = d;
		Program lp(CGAL::SMALLER, false, 0, false, 0);		

		for(int n=0; n<N; n++){
			int sum = 0;
			for(int i=0; i<d; i++){
				int a_i; std::cin >> a_i;
				lp.set_a(i, n, a_i);
				sum += a_i*a_i;
			}
			int b_i; std::cin >> b_i;
			lp.set_a(R, n, sqrt(sum));
			lp.set_b(n, b_i);
		}

		lp.set_l(R, true, 0);
		// time -1 to maximize instead of minimazing
		lp.set_c(R, -1);

		Solution s = CGAL::solve_linear_program(lp, ET());
		if(s.is_infeasible()){
			std::cout << "none\n"; 
		}else if(s.is_unbounded()){
			std::cout << "inf\n";
		}else{
			assert(s.is_optimal());
			std::cout << floor_to_double(-1 * s.objective_value()) + 0.0 << "\n";
		}
		
		std::cin >> N;
	}
	return 0;
}
