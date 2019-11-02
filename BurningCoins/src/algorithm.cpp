#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

void print_matrix(int n, vector<vector<int>> a) {
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

int burning_coins(int n, vector<int> &a) {

    vector<vector<int>> gain(n, vector<int>(n, 0));
    // gain[i, j] is the guaranteed gain we can get with the coins [i, j] when its our turn

    // base case: lenght [i, j] of 1 
    for (int i = 0; i<n; i++) {
        gain[i][i] = a[i]; 
    }

    // length of 2
    for (int i=0; i<n-1; i++) {
        int j = i + 1;
        gain[i][j] = max(a[i], a[j]);
    }

    // length of more than 2
    for (int l=3; l<=n; l++) {
        for (int i=0; i<n-l+1; i++) {
            int j = i + l - 1;

                int pick_first = a[i] + min(gain[i+2][j], gain[i+1][j-1]);
                int pick_last = a[j] + min(gain[i+1][j-1], gain[i][j-2]);
                gain[i][j] = max(pick_first, pick_last);
          
        }
    }

    return gain[0][n-1];

}

int main() {

    int t;
    cin >> t;
    while(t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i=0; i<n; i++) {
            cin >> a[i];
        }
        int result = burning_coins(n, a);
        cout << result << endl;
    }

    return 0;
}
