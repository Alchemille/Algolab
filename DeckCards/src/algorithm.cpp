#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

/*
idea for O(n) solution:
Sliding window. Two pointers. Like slides week 1. 
*/

void deck_cards(int n, int k, vector<int> &a) {

    vector<vector<int>> partial_sums(n, vector<int>(n, 0));

    // compute partial 2D partial sums and choice with DP
    for (int i=0; i<n; i++) {
        partial_sums[i][i] = a[i];
    }

    int best_choice = a[0];
    int best_i = 0;
    int best_j = 0;

    for (int i=0; i<n; i++) {
        
        // compute partial sums
        for (int j=i+1; j<n; j++) {
            partial_sums[i][j] = partial_sums[i][j-1] + a[j];
        }

        // compute absolute diff and best choice
        for (int j=i; j<n; j++) {

            int diff = abs(k - partial_sums[i][j]);
            if (diff < best_choice) {
                best_choice = diff;
                best_i = i;
                best_j = j;
            }

            //cout << i << " " << j << " " << diff << " " << best_choice << " " << best_i << " " << best_j << " \n";
        
        }
    }

    cout << best_i << " " << best_j << "\n";

}

int main(int argc, char const *argv[]) {

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {

        int n, k;
        cin >> n;
        cin >> k;

        vector<int> a(n, 0);

        for (int i=0; i<n; i++) {
            cin >> a[i];
        }

        deck_cards(n, k, a);
    }

    return 0;
}
