#include <bits/stdc++.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef std::vector<std::pair<P, P>> Tr;

bool check(P p, P q, P ref, P v){
	if(CGAL::collinear(p, q, v)) return true;
	if(CGAL::left_turn(p, q, ref))
		return CGAL::left_turn(p, q, v);
	else
		return !CGAL::left_turn(p, q, v);
}

bool is_contained(Tr &tr, P v){
	assert(tr.size() == 3);
	for(int i=0; i<3; i++){
		if(!check(tr[i].first, tr[i].second, tr[(i+1)%3].first, v))
			return false;
	}
	return true;
}

void testcase(){
	int m, n; std::cin >> m >> n;
	std::vector<P> path(m);
	for(int i=0; i<m; i++)
		std::cin >> path[i];
	std::vector<Tr> triangles(n, Tr(3));
	for(int i=0; i<n; i++){
		std::cin >> triangles[i][0].first >> triangles[i][0].second
			>> triangles[i][1].first >> triangles[i][1].second
				>> triangles[i][2].first >> triangles[i][2].second;
	}

	// Compute which legs are contained in which maps
	std::vector<std::vector<bool>> inside(n, std::vector<bool>(m-1, false));
	for(int i=0; i<n; i++){
		for(int j=0; j<m-1; j++){
			if(is_contained(triangles[i], path[j])
				&& is_contained(triangles[i], path[j+1]))
				inside[i][j] = true;
		}
	}

	// Sliding window
	int i=0, j=0;
	std::vector<int> contained(m-1, 0);
	int n_contained = 0;
	int best_size = std::numeric_limits<int>::max();
	while(j<n && i<n && i<=j){
		if(n_contained == m-1){
			if(j-i < best_size)
				best_size = j-i;
			for(int jd=0; jd<m-1; jd++){
				if(inside[i][jd]){
					contained[jd]--;
					if(contained[jd] == 0)
						n_contained--;
				}
			}
			i++;
		}else{
			j++;
			if(j<n){
				for(int jd=0; jd<m-1; jd++){
					if(inside[j][jd]){
						if(contained[jd] == 0)
							n_contained++;
						contained[jd]++;
					}
				}
			}
		}
		if(n_contained == m-1)
			if(j-i < best_size)
				best_size = j-i;
	}
	std::cout << best_size << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
