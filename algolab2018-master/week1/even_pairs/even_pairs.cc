#include <iostream>
#include <vector>

int main(int argc, char** argv){
    int n; std::cin >> n;

    std::vector<int> numbers;

    for(int i = 0; i < n; i++){
        int m;
        int e;
        std::cin >> m;
        for(int j = 0; j < m; j++){
            std::cin >> e;
            numbers.push_back(e);
        }

        // Algorithms to compute the number of even pairs
        std::vector<int> partial_sums;
        partial_sums.push_back(numbers[0]);
        for(int j = 0; j < m - 1; j++){
            partial_sums.push_back(partial_sums[j] + numbers[j+1]);
        }

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

        std::cout << number_pairs << std::endl;

        // Cleaning
        numbers.clear();
    }

    return 0;
}