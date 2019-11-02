#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

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
		Traits::Circle c = mc.circle();
		CORE::Expr d = CGAL::sqrt(mc.circle().squared_radius());
		std::cout << CGAL::ceil((K::FT) d) << "\n";
		std::cin >> n;
	}
	std::cout << std::flush;
	return 0;
}
