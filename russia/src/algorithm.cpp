#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <assert.h>

using namespace std;


void best_gain_efficient(int n, int m, int k, vector<int> &coins) {

    vector<vector<int>> gains(n, vector<int>(n, 0));
    vector<int> positions_1d(n, 0);

    // fill position (round number) when l coins left
    positions_1d[n-1] = k;
    for (int l=n-2; l>=0; l--) {
        positions_1d[l] = (positions_1d[l+1]==0)? m-1 : positions_1d[l+1]-1;
    }

    // lenght of 1
    if (positions_1d[0]==0) for (int i=0; i<n; i++) gains[i][i] = coins[i];

    // length of 2
    for (int i=0; i<n-1; i++) {
        if (positions_1d[1]==0) gains[i][i+1] = max(coins[i], coins[i+1]);
        else gains[i][i+1] = min(gains[i][i], gains[i+1][i+1]);
    }

    //fill gains 2d matrix
    for (int l=3; l<=n; l++) {
        for (int i=0; i<n-l+1; i++) {
            
            int j = i+l-1;

            if (positions_1d[l-1] == 0) {
                int pickfirst = coins[i] + gains[i+1][j]; 
                int picklast =  coins[j] + gains[i][j-1];
                gains[i][j] = max(pickfirst, picklast);
            }
            else {
                gains[i][j] = min(gains[i+1][j], gains[i][j-1]);
            }
        }
    }
    //printMatrix(gains, n);
    cout << gains[0][n-1] << "\n";

}


int main() {

    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;

    while(t--) {

        int n, m, k;
        cin >> n >> m >> k;

        vector<int> coins(n, 0);
        for (int i=0; i<n; i++) {            
            cin >> coins[i];
        }

        best_gain_efficient(n, m, k, coins);
    }

    return 0;
}
