#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

/*
THIS IDEA DOES NOT WORK. SEE sol0 and sol1.
*/

void print_matrix(int n, vector<vector<long>> &a) {
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

long nChoosek(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    long result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

int even_matrices(int n, vector<vector<long>> &a) {

    vector<long> results(n, 0); // vector with partial solutions to be summed, for fixed i1, i2

    // compute partial sums over diagonal blocks (reduction to even pairs)
    vector<vector<long>> partial_sums(n, vector<long>(n, 0));
    int O = 0;
    int E = 0;

    // base case
    int sum_line = 0;
    for (int col=0; col<n; col++) {
        sum_line += a[0][col];
        partial_sums[0][col] = sum_line;
    }

    for (int i = 1; i < n; i ++) {
        sum_line = 0;
        for (int j = 0; j < n; j ++) {
            sum_line += a[i][j];
            partial_sums[i][j] = partial_sums[i - 1][j] + sum_line;
        }
    }

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            if (partial_sums[i][j] % 2 == 0) {
                E ++;
            }
            else O ++;
        }
    }

    long solution = E + nChoosek(E, 2) + nChoosek(E, 2) * nChoosek(O, 2) + nChoosek(E, 4);

    return solution;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while(t--) {
        int n;
        cin >> n;
        vector<vector<long>> a(n, vector<long>(n, 0));

        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                cin >> a[i][j];
            }
        }

        int result = even_matrices(n, a);
        cout << result << endl;
    }

    return 0;
}
