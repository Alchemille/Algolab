#include <iostream>
#include <limits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;

typedef CGAL::Quadratic_program<int64_t> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(SolT s){
	double a = std::ceil(CGAL::to_double(s));
	while(a < s) a++;
	while(a+1 < s) a--;
	return a;
}

int main(){
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(10);
	int64_t T; std::cin >> T;
	for(int64_t t=0; t<T; t++){
		Program lp(CGAL::SMALLER, true, 1, false, 0);

		int64_t n, m, h, w;
		std::cin >> n >> m >> h >> w;
		std::vector<std::pair<int, int>> new_points; 
		for(int64_t i=0; i<n; i++){
			int64_t a, b;
			std::pair<int, int> pair;
			std::cin >> a >> b;
			new_points.push_back(std::make_pair(a, b));
		}
		int64_t idx = 0;
		for(int64_t i=0; i<n-1; i++){
			for(int64_t j=i+1; j<n; j++){
				int64_t a_x, a_y, b_x, b_y;
				std::tie(a_x, a_y) = new_points[i];
				std::tie(b_x, b_y) = new_points[j];
				
				if(w * std::abs(a_y - b_y) <= h * std::abs(a_x - b_x)){
					// sides are going to collide
					lp.set_a(i,idx,w);
					lp.set_a(j,idx,w);
					lp.set_b(idx, 2*std::abs(new_points[i].first - new_points[j].first));
				}else{
					// tops are going to collide
					lp.set_a(i,idx,h);
					lp.set_a(j,idx,h);
					lp.set_b(idx, 2*std::abs(new_points[i].second - new_points[j].second));
				}
				++idx;
			}
		}
		for(int64_t i=0; i<n; i++){
			lp.set_c(i, -2*(h+w));
		}
		std::vector<std::pair<int, int>> old_pos(m);
		for(int i=0; i<m; i++)
			std::cin >> old_pos[i].first >> old_pos[i].second;
		
		for(int i=0; m>0 && i<n; i++){
			int64_t min_cst = std::numeric_limits<int64_t>::max();
			int64_t a_x, a_y, b_x, b_y;
			std::tie(a_x, a_y) = new_points[i];
			for(int j=0; j<m; j++){
				std::tie(b_x, b_y) = old_pos[j];
				if(w*abs(a_y - b_y) <= h*abs(a_x - b_x)){
					min_cst = std::min(min_cst, h*(2*abs(a_x - b_x) - w));	
				}else{
					min_cst = std::min(min_cst, w*(2*abs(a_y - b_y) - h));	
				}
			}
			lp.set_a(i, idx, h*w);
			lp.set_b(idx, min_cst);
			++idx;
		}

		CGAL::Quadratic_program_options options;
        options.set_pricing_strategy(CGAL::QP_BLAND);
        Solution s = CGAL::solve_linear_program(lp, ET(), options);

		if(s.is_infeasible()){
			std::cout << "none\n";
		}else{
			std::cout << ceil_to_double(-1 * s.objective_value()) << "\n";
		}
		
	}
	return 0;
}
