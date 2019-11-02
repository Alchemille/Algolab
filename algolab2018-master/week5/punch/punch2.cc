#include <bits/stdc++.h>

void testcase(){
	int n, k; std::cin >> n >> k;
	std::vector<int> prices(n), volumes(n);
	for(int i=0; i<n; i++)
		std::cin >> prices[i] >> volumes[i];	
	std::vector<std::vector<int>> mem_table(k+1, std::vector<int>(n, 0));
	std::vector<std::vector<int>> na(k+1, std::vector<int>(n, 0));
	// set the base cases of the table
	for(int i=0; i<k+1; i++){
		mem_table[i][0] = ceil((double)i / volumes[0]) * prices[0];
		na[i][0] = 1;
	}
	for(int i=1; i<k+1; i++){
		for(int j=1; j<n; j++){
			// special case: force to use the jth alcohol
			int price_with = 0;
			int best_price = 0;
			int n_with = 0, n_without = 0;
			int best_drinks = 0;

			if(i <= volumes[j]){
				price_with = mem_table[i][j] = prices[j];
				n_with = 1;
			}else{
				price_with = prices[j] + mem_table[i-volumes[j]][j];
				n_with = na[i-volumes[j]][j];

				if(price_with == prices[j] + mem_table[i-volumes[j]][j-1]){
					n_with = std::max(n_with, 1+na[i-volumes[j]][j-1]);
				}
			}

			int price_without = mem_table[i][j-1];
			n_without = na[i][j-1];

			if(price_with < price_without){
				best_price = price_with;
				best_drinks = n_with;	
			}else if(price_with == price_without){
				best_price = price_with;
				best_drinks = std::max(n_with, n_without);
			}else{
				best_price = price_without;
				best_drinks = n_without;
			}
			mem_table[i][j] = best_price;
			na[i][j] = best_drinks;
		}
	}
	std::cout << mem_table[k][n-1] << " " << na[k][n-1] << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int T; std::cin >> T;
	while(T--)
		testcase();
	return 0;
}
