#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

double ceilN(const K::FT &x){
	double a = std::ceil(CGAL::to_double(x));	
	while (a < x) a += 1;
	while (a -1 >= x) a -= 1;
	return a;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(0);
	int n; std::cin >> n;
	while(n > 0){
		std::vector<P> population;
		for(int i = 0; i < n; i++){
			long p, q;
			std::cin >> p;
			std::cin >> q;
			population.push_back(P(p, q));
		}
		Min_circle mc(population.begin(), population.end(), true);
		CORE::Expr min_d = mc.circle().squared_radius();

		for(int i=0;  i < mc.number_of_support_points(); i++){
			P sup_p = mc.support_point(i);
			std::vector<P> without;
			for(int j=0; j < n; j++){
				if(population[j] != sup_p){
					without.push_back(population[j]);
				}
			}

			Min_circle mc2(without.begin(), without.end(), true);
			Traits::Circle c = mc2.circle();
			if(c.squared_radius() < min_d)
				min_d = c.squared_radius();
		}

		std::cout << (long) ceilN(CGAL::sqrt(min_d)) << std::endl;
		std::cin >> n;
	}
	std::cout << std::flush;
	return 0;
}
