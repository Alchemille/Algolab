#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Tr;
typedef K::Point_2 P;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;
#include <CGAL/MP_Float.h>

//typedef CGAL::MP_Float ET;
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

bool solve_linear_program(int I, std::vector<P> &mpe, std::vector<P> &sensors, std::vector<int> &energies, std::vector<double> &max_r){
	int m = mpe.size();
	Program lp(CGAL::SMALLER, true, 0, false, 0);
	for(int i=0; i<mpe.size(); i++){
		// find r_i by finding distance to closest human;
		for(int j=0; j<sensors.size(); j++){
			double d = CGAL::squared_distance(sensors[j], mpe[i]);
			if(d < max_r[i]){
				lp.set_a(i, j, -1/d);
			}
		}
		lp.set_c(i, 1);
		lp.set_a(i, sensors.size()+1, 1);
	}
	lp.set_b(sensors.size()+1, I);

	for(int j=0; j<sensors.size(); j++){
		lp.set_b(j, -energies[j]);
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
//	CGAL::print_linear_program(std::cerr, lp);
	if(sol.is_infeasible()){
		return false;
	}else{
		return sol.objective_value() <= I;
	}
}

void testcase(){
	int n, m, h, I; std::cin >> n >> m >> h >> I;
	std::vector<P> sensors(n);
	std::vector<int> energies(n);
	for(int i=0; i<n; i++){
		std::cin >> sensors[i] >> energies[i];
	}
	std::vector<P> mpe(m);
	for(int i=0; i<m; i++)
		std::cin >> mpe[i];
	std::vector<P> humans(h);
	for(int i=0; i<h; i++)
		std::cin >> humans[i];
	Tr tr; tr.insert(humans.begin(), humans.end());
	std::vector<double> max_r(m, std::numeric_limits<double>::max());
	if(h > 0)
		for(int i=0; i<m; i++){
			max_r[i] = CGAL::squared_distance(mpe[i], tr.nearest_vertex(mpe[i])->point()); 
		}
	
	// binary search algo to find shortest k
	
	int i=0;
	int j=mpe.size();
	bool s = false;
	
	while(i != j && j-i > 1){
		int m = (j+i)/2;
		auto mpe_window = std::vector<P>(mpe.begin(), mpe.begin()+m);
		auto max_r_window = std::vector<double>(max_r.begin(), max_r.begin()+m);
		bool trial = solve_linear_program(I, mpe_window, sensors, energies, max_r_window);
		if(trial){
			s = true;
			j = m;
		}else{
			i = m;
		}
	}
	if(i != j){
		auto mpe_window = std::vector<P>(mpe.begin(), mpe.begin()+j);
		auto max_r_window = std::vector<double>(max_r.begin(), max_r.begin()+j);
		bool trial = solve_linear_program(I, mpe_window, sensors, energies, max_r_window);
		if(trial)
			s = trial;
		mpe_window = std::vector<P>(mpe.begin(), mpe.begin()+i);
		max_r_window = std::vector<double>(max_r.begin(), max_r.begin()+i);
		trial = solve_linear_program(I, mpe_window, sensors, energies, max_r_window);
		if(trial){
			s = true;
			j=i;
		}else
			i=j;
	}

	if(!s)
		std::cout << "impossible\n";
	else
		std::cout << i << "\n";

/*
	bool s = false;
	for(int i=0; i<=m; i++){
		auto mpe_window = std::vector<P>(mpe.begin(), mpe.begin()+i);
		auto max_r_window = std::vector<double>(max_r.begin(), max_r.begin()+i);
		bool trial = solve_linear_program(I, mpe_window, sensors, energies, max_r_window);
		if(trial){
			s = true;
			std::cout << i << "\n";
			break;
		}
	}
	if(!s)
		std::cout << "impossible\n";
*/
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
