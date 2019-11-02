#include <bits/stdc++.h>

void testcase(){
	int n, i, k; std::cin >> n >> i >> k;
	std::vector<int> modular_classes(k, 0);
	for(int i=0; i<n; i++){
		int height; std::cin >> height;
		std::vector<int> updates(k, 0);
		for(int j=0; j<k; j++){
			if(modular_classes[j]){
				updates[(j+height) % k] = 1;
			}
		}
		for(int j=0; j<updates.size(); j++)
			if(updates[j])
				modular_classes[j] = 1;
		modular_classes[height % k] = 1;
	}
	if(modular_classes[i])
		std::cout << "yes\n";
	else
		std::cout << "no\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
