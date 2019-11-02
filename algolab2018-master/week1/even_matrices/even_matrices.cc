#include <iostream>

int main(){
    // parser
    int n; std::cin >> n;
    for(int i = 0; i<n; i++){
		std::ios_base::sync_with_stdio(false);
        int m; std::cin >> m;
		
		int** matrix = new int*[m];
		int** partial_sum = new int*[m];
		for(int i=0; i<m; i++){
			matrix[i] = new int[m];
			partial_sum[i] = new int[m];
		}

        for(int k = 0; k < m; k++){
            for(int l = 0; l < m; l++){
                int e; std::cin >> e;
                matrix[k][l] = e;
            }
        }

        for(int k = 0; k < m; k++){
            for(int l = 0; l < m; l++){
                int val = matrix[k][l];
                if(k != 0)
                    val += partial_sum[k-1][l];
                if(l != 0)
                    val += partial_sum[k][l-1];
                if(k != 0 && l != 0)
                    val -= partial_sum[k-1][l-1];

                partial_sum[k][l] = val;
            }
        }
		int nb_even = 0;
		// straight-forwardly count the number of such pairs
		for(int i1=0; i1<m; i1++){
			for(int i2=i1; i2<m; i2++){

			int number_even = 0;
			int number_odd = 0;

			for(int j = 0; j < m; j++){
				if(partial_sums[j] % 2 == 0){
					number_even++;
				}else{
					number_odd++;
				}
			}

			int number_pairs = (number_even * (number_even - 1)) / 2;
			number_pairs += (number_odd * (number_odd - 1)) / 2;

			// number of pairs with the first element
			number_pairs += number_even;

			}
		}
		std::cout << nb_even << "\n";
	}
    return 0;
}
