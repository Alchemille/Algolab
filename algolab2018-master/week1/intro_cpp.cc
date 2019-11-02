#include <iostream>
#include <iomanip>
#include <vector>
#include <bits/stdc++.h>

int main(){
    int n; std::cin >> n;
    for(int i = 0; i < n; i++) {
        int m; std::cin >> m;
        std::vector<int> numbers;

        for(int j = 0; j < m; j++){
            int e; std::cin >> e;
            numbers.push_back(e);
        }

        int idx_to_remove; std::cin >> idx_to_remove;

        numbers.erase(numbers.begin() + idx_to_remove);

        int idx_min, idx_max; std::cin >> idx_min; std::cin >> idx_max;

        numbers.erase(numbers.begin() + idx_min, numbers.begin() + idx_max + 1);

        for(auto &e : numbers)
            std::cout << e << " ";

        std::cout << std::endl;
     }
    return 0;
}