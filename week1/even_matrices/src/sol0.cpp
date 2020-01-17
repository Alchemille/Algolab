#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

/*
partial_sums: 
ici, partial sums are diagonale block
in sol1.cpp, are columns subsums.
*/

void print_matrix(int n, vector<vector<long>> &a) {
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}

int nChoosek(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
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

    //print_matrix(n, partial_sums);

    int solution = 0;

    for (int i1=0; i1<n; i1++) {
        for (int i2=i1; i2<n; i2++) {

            // i1 and i2 fixed. Use column partial sums to get sum for each col
            // each col is like a single cell in the 1D case
            int even = 0;
            int odd = 0;

            for (int col=0; col<n; col++) {

                int sum_block;
                if (i1 == 0) {
                    sum_block = partial_sums[i2][col];
                }
                else sum_block = partial_sums[i2][col] - partial_sums[i1-1][col]; // donne un block!

                if (sum_block % 2 == 0) even ++;
                else odd ++;

            }

            //cerr << i1 << " " << i2 << " " << even << " " << odd << "\n";

            int sol_block = 0;
            sol_block += nChoosek(even, 2); 
            sol_block += nChoosek(odd, 2); 
            sol_block += even;

            solution += sol_block;

        }
    }

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
