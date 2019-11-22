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

void deck_cards_n(int n, int k, vector<int> &a) {

    int i = 0;
    int j = 0;
    int best_i = 0;
    int best_j = 0;
    int best_sum = a[0];
    int current_sum = a[0];

    while(best_sum != k && j < n-1) {

        //cout << current_sum << " " << best_sum << " " << best_i << " " << best_j << " " << i << " " << j << "\n";

        if (k > current_sum) {
            //cout << "cond 1\n";
            j ++;
            current_sum += a[j];
            if (abs(k-current_sum) < abs(k-best_sum)) {
                best_sum = current_sum;
                best_j = j;
                best_i = i;
            }
        }
        else if (k < current_sum && i == j) {
            //cout << "cond 2\n";
            i++; j++;
            current_sum = a[i];
            if (abs(k-current_sum) < abs(k-best_sum)) {
                best_sum = current_sum;
                best_i = i;
                best_j = j;
            }
        }
        else if (k < current_sum) {
            //cout << "cond 3\n";
            current_sum -= a[i];
            i ++;
            if (abs(k-current_sum) < abs(k-best_sum)) {
                best_sum = current_sum;
                best_i = i;
                best_j = j;
            }
        }
    }

    cout << best_i << " " << best_j << "\n";

}

void deck_cards_n2(int n, int k, vector<int> &a) {

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

        deck_cards_n(n, k, a);
    }

    return 0;
}
