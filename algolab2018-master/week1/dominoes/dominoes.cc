#include <iostream>
#include <vector>

int main(){
    // Parser
    int n; std::cin >> n;
    for(int i = 0; i<n; i++){
        int m; std::cin >> m;
        std::vector<int> heights;

        for(int j = 0; j < m; j++){
            int e; std::cin >> e;
            heights.push_back(e);
        }

        std::vector<int> falling (m);
        std::fill(falling.begin(), falling.end(), 0);

        // compute the value in a DP fashion.
        for(int idx = m-1; idx >= 0; idx--){
            // Find greater spanning domino
            int h = heights[idx];
            int greater_spanning_idx = -1;
            int greater = 0;

            int falling_pieces = 1;
            int stop = idx;

            for(int k = idx+1; k < std::min(idx + h, m); k++){
                if(idx + h > k + heights[k]){
                    falling_pieces++;
                }else{
                    stop = k;
                    break;
                }
            }

            /*if(greater_spanning_idx >= 0) {
                int falling_not_in_range = greater_spanning_idx - idx - 1;
                falling[idx] = 1 + falling_not_in_range + falling[greater_spanning_idx];
            }*/

            falling[idx] = falling_pieces + falling[stop];
        }
        std::cout << falling[0] << std::endl;
    }
    return 0;
}