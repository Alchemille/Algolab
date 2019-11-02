#include <iostream>
#include <vector>

int main(int argc, char** argv){
    // Parsing the input data
    int n; std::cin >> n;

    std::vector<int> numbers;

    // Loop over different cases
    for(int i = 0; i < n; i++){
        int m;
        int e;
        std::cin >> m;
        for(int j = 0; j < m; j++){
            std::cin >> e;
            numbers.push_back(e);
        }

        int s = 0;
        for(auto &k : numbers){
            s += k;
        }

        std::cout << s << std::endl;

        numbers.clear();
    }
    return 0;
}