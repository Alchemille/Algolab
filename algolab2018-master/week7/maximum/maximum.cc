#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(SolT n){
	double a = std::floor(CGAL::to_double(n));
	while(a > n)
		a--;
	while(a+1 <= n)
		a++;
	return a;
}

double ceil_to_double(SolT n){
	double a = std::floor(CGAL::to_double(n));
	while(a < n)
		a++;
	while(a-1 >= n)
		a--;
	return a;
}

int main(){
	int p; std::cin >> p;
	while(p != 0){
		int a, b;
		std::cin >> a >> b;
		if(p == 1){
			const int X = 0;
			const int Y = 1;
			Program lp(CGAL::SMALLER, true, 0, false, 0);
			lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
			lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); // 4x+ 2y <= ab
			lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); //-x+y <= 1
			
			lp.set_d(X, X, 2*a); // WARNING: don't forget the 2 for squared variables!!!
			lp.set_c(Y, -b);
			Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
			assert(s.solves_quadratic_program(lp));

			if(s.is_unbounded()){
				std::cout << "unbounded\n";
			}else if(s.is_infeasible()){
				std::cout << "no\n";
			}else{
				assert(s.is_optimal());
				auto opt = s.variable_values_begin();
				std::cout << std::setprecision(12) << floor_to_double(-1 * s.objective_value()) << "\n";
			}
		}else if (p==2){
			const int X = 0;
			const int Y = 1;
			const int Z = 2;

			Program qp(CGAL::SMALLER, false, 0, false, 0);

			qp.set_a(X, 0, -1); qp.set_a(Y, 0, -1); qp.set_b(0, 4); //-x-y<= 4
			qp.set_a(X, 1, -4); qp.set_a(Y, 1, -2); qp.set_a(Z, 1, -1); qp.set_b(1, a*b);
			qp.set_a(X, 2, 1); qp.set_a(Y, 2, -1); qp.set_b(2, 1);

			qp.set_d(X, X, 2*a);
			qp.set_d(Z, Z, 2);
			qp.set_c(Y, b);

			qp.set_u(X, true, 0);
			qp.set_u(Y, true, 0);

			Solution s = CGAL::solve_quadratic_program(qp, ET());
			assert(s.solves_quadratic_program(qp));

			if(s.is_unbounded()){
				std::cout << "unbounded\n";
			}else if(s.is_infeasible()){
				std::cout << "no\n";
			}else{
				assert(s.is_optimal());
				auto opt = s.variable_values_begin();
				std::cout << std::setprecision(12) << ceil_to_double(s.objective_value()) << "\n";
			}
		}
		std::cin >> p;
	}
	return 0;
}
