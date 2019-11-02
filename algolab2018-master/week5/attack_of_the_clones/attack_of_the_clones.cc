#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <deque>
#include <tuple>

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		int n, m;
		std::cin >> n >> m;
		std::vector<std::pair<int, int>> protected_segments(n);
		for(int i=0; i<n; i++){
			int a, b;
			std::cin >> a >> b;
			protected_segments[i] = std::make_pair(a-1, b-1); // correct to 0-indexed ring
		}
		// construct a map seg -> number of jedi
		std::sort(protected_segments.begin(), protected_segments.end());

		std::vector<std::pair<int, int>> L;
		for(int i=0; i<n; i++){
			L.push_back(std::make_pair(protected_segments[i].first, 1));
			L.push_back(std::make_pair((protected_segments[i].second + 1)%m, -1));
		}
		std::sort(L.begin(), L.end());
		int sum = 0;
		int min_index = 0;
		int min_value = std::numeric_limits<int>::max();
		int curr_idx = 0;

		for(int i=0; i<L.size(); i++){
			if(L[i].first == curr_idx){
				sum += L[i].second;
			}else{
				if(sum < min_value){
					min_value = sum;
					min_index = curr_idx;
				}
				sum += L[i].second;
				curr_idx = L[i].first;
			}
		}
		int min_seg = min_index;
		//std::cerr << "min seg = " << min_index << "\n";
		int best_n = 0;
		std::vector<int> jedi_on_min_seg;
		for(int i=0; i<n; i++){
			int a, b;
			std::tie(a, b) = protected_segments[i];
			if(a < b && a <= min_seg && min_seg <= b)
				jedi_on_min_seg.push_back(i);
			if(a > b && a <= min_seg && min_seg >= b)
				jedi_on_min_seg.push_back(i);
		}
		for(auto &j: jedi_on_min_seg){
			//std::cerr << "considering: " << j << "\n";
			int min_seg_off = protected_segments[j].first;
			int left_lim = std::max(m, (protected_segments[j].first - min_seg_off + m) % m);
			int right_most = (protected_segments[j].second - min_seg_off + m) % m;

			int n_jedi = 1;

			for(int i=0; i<n; i++){
				int idx = (i + j + n) % n;
				int a = (protected_segments[idx].first - min_seg_off + m) % m;
				int b = (protected_segments[idx].second - min_seg_off + m) % m;
				// TODO: problem with one elements overlaps
				if(a <= b && a != 0){
					// Can be fitted
					if(b < left_lim){
						if(a > right_most){
							//std::cerr << "adding seg number : " << i << ", b = " << b << "\n";
							right_most = b;
							n_jedi++;
						}else{
							if(b < right_most)
								right_most = b;
						}
					}
				}
			}

			if(n_jedi > best_n)
				best_n = n_jedi;
		}
		//std::cerr << "best case with seg = " << best_n << "\n";
		// special case: no protection on min_seg
		int left_lim = m;
		int right_most = 0;
		int n_jedi = 0;
		std::vector<std::pair<int, int>>::iterator start_point = std::lower_bound(protected_segments.begin(), protected_segments.end(), std::make_pair(min_seg, 0));
		int start = std::distance(protected_segments.begin(), start_point);

		for(int i=0; i<n; i++){
			int idx = (i + start + n) % n;
			int a = (protected_segments[idx].first - min_seg + m) % m;
			int b = (protected_segments[idx].second - min_seg + m) % m;
			if(a <= b && a != 0){
				// Can be fitted
				if(b < left_lim){
					if(a > right_most){
						right_most = b;
						n_jedi++;
					}else{
						if(b < right_most)
							right_most = b;
					}
				}
			}
		}

		if(n_jedi > best_n)
			best_n = n_jedi;
	
		std::cout << best_n << "\n";
	}
	return 0;
}
