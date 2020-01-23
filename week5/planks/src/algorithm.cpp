#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <math.h>

using namespace std;

/*
https://www.geeksforgeeks.org/vector-insert-function-in-c-stl/
https://stackoverflow.com/questions/2551775/appending-a-vector-to-a-vector
https://stackoverflow.com/questions/13324431/c-vectors-insert-push-back-difference
https://stackoverflow.com/questions/3863282/checking-whether-a-vector-is-empty
https://www.geeksforgeeks.org/stdremove-stdremove_if-c/
https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
https://stackoverflow.com/questions/347441/erasing-elements-from-a-vector

Instead of storing decisions as vector of indices used, could have used vector of boolean of size n
*/

void print_vector(vector<int>& v) {
    for(int i=0; i<v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << "\n";
}

vector<int>  subset_sum(vector<int> &set, int n, int k) {

    // implement DP that memorize decision
    // return vector of indices of set used to reach k.

    vector<vector<bool>> dp_result(n+1, vector<bool>(k+1));
    vector<vector<vector<int>>> dp_decisions(n+1, vector<vector<int>>(k+1, vector<int>()));

    // initialization
    for (int i=0; i<n+1; i++) dp_result[i][0] = true;
    for (int j=1; j<k+1; j++) dp_result[0][j] = false;
    
    //recursiion dp bottom up
    for (int i=1; i<n+1; i++) {
        for (int j=1; j<k+1; j++) {
                        
            // store decision
            if (dp_result[i-1][j]) {
                dp_decisions[i][j].insert(dp_decisions[i][j].end(), dp_decisions[i-1][j].begin(), dp_decisions[i-1][j].end());
                dp_result[i][j] = true;
            }
            else if (j-set[i-1] >= 0 && dp_result[i-1][j-set[i-1]]) { // evaluation from left to right
                dp_decisions[i][j].insert(dp_decisions[i][j].end(), dp_decisions[i-1][j-set[i-1]].begin(), dp_decisions[i-1][j-set[i-1]].end());
                dp_decisions[i][j].push_back(i-1);
                dp_result[i][j] = true;
            }
        }
    }

    return dp_decisions[n][k];
}

void squares() {
    int n;
    cin >> n;
    vector<int> lengths(n, 0);

    // read input
    for (int i=0; i<n; i++) cin >> lengths[i];

    // compute contribution of each side
    int sum = 0;
    for(vector<int>::iterator it = lengths.begin(); it != lengths.end(); ++it) {
        sum += *it;
    }
    float k = sum / 4; // sum should be multiple of 4
    if (k == std::floor(k)) k = (int)k; 
    else {
        cout << 0 << "\n";
        return;
    }
    
    // search subset sum 4 times
    vector<int> dp_decisions_one_side;
    int n_left = n;

    for (int i=0; i<4; i++) {

        dp_decisions_one_side = subset_sum(lengths, n_left, k);
        //cout << "decision " << dp_decisions_one_side.empty() << "\n";

        if (!dp_decisions_one_side.empty()) {
            
            n_left -= dp_decisions_one_side.size();

            // remove indices of dp_decisions_one_side from lengths
            for (std::vector<int>::iterator it = dp_decisions_one_side.begin(); it!=dp_decisions_one_side.end(); ++it) {
                lengths[*it] = -1;
            }
            // if set to 0 above, lines below optional
            std::vector<int>::iterator separation = std::remove(lengths.begin(), lengths.end(), -1);
            lengths.erase(separation, lengths.end());
        }
        else {
            cout << "0\n";
            return;
        }
    }
    cout << "1\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) squares();
}
