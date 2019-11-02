#include <bits/stdc++.h>
std::vector<int> get_vector_from_int(std::vector<int> v, int s){
	std::vector<int> res;
	for(int i=0; i < v.size(); i++){
		if(s & (1 << i)){
			res.push_back(v[i]);
		}
	}
	return res;
}
std::vector<std::pair<int, int>> possible_sums(std::vector<int> v){
	// at least one element
	std::vector<std::pair<int, int>> possible_sums;

	for(int s=1; s < (1 << v.size()); s++){
		int sum = 0;
		for(int i=0; i<v.size(); i++){
			if(s & 1 << i){
				sum += v[i];
			}
		}
		possible_sums.push_back(std::make_pair(sum, s));
	}
	return possible_sums;
}

void testcase(){
	int n; std::cin >> n;
	std::vector<int> lengths(n);
	uint64_t sum = 0;
	for(int i=0; i<n; i++){
		int l; std::cin >> l;
		lengths[i] = l; sum += l;
	}

	int middle = n/2;
	std::vector<int> list_a = std::vector<int>(lengths.begin(), lengths.begin() + middle);
	std::vector<int> list_b = std::vector<int>(lengths.begin() + middle, lengths.end());
	
	// compute the number of sets that sum to L/2
	std::vector<std::pair<int, int>> r_a = possible_sums(list_a);
	std::vector<std::pair<int, int>> r_b = possible_sums(list_b);
	
	std::sort(r_a.begin(), r_a.end());
	std::sort(r_b.begin(), r_b.end());
	int cnt = 0;
	int i=0, j=r_b.size()-1;
	while(i < r_a.size() && j>=0){
		auto p_a = r_a[i];
		auto p_b = r_b[j];
		auto s = p_a.first + p_b.first;
		if(s == sum / 2){
			//recurse the procedure for L/4 on the two sublists
			// let's first try with straight-forward checks
			std::vector<int> l_a_a = get_vector_from_int(list_a, p_a.second);
			std::vector<int> l_a_b = get_vector_from_int(list_b, p_b.second);
			for(auto v: l_a_b)
				l_a_a.push_back(v);

			std::vector<std::pair<int, int>> possible_sums_a = possible_sums(l_a_a);

			std::vector<int> l_b_a = get_vector_from_int(list_a, ~p_a.second);
			std::vector<int> l_b_b = get_vector_from_int(list_b, ~p_b.second);
			for(auto v: l_b_b)
				l_b_a.push_back(v);
			std::vector<std::pair<int, int>> possible_sums_b = possible_sums(l_b_a);
			int cnt1 = 0, cnt2 = 0;
			for(auto p: possible_sums_a)
				if(p.first == sum/4) cnt1++;
			for(auto p: possible_sums_b)
				if(p.first == sum/4) cnt2++;
			cnt += cnt1*cnt2;
			i++;
			j--;
		}else if(s < sum / 2){
			i++;
		}else if(s > sum / 2){
			j--;
		}
	}
	
	std::cout << cnt << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++){
		testcase();
	}
}
