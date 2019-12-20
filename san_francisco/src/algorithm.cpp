#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>
#include <string>
#include <set>
#include <tuple>

using namespace std;

/*
Idea: 
Dynamic programming. 
DP[i, j] the score a player can get with i remaining turns and starting from node j.
Important: Seems need to use bottom up to stop incrementing steps used if the score is reached
before the max number of steps, for starting node 0.
Other trick allowed by bottom up: if X is cul-de-sac, set DP[i, X] = DP[i, X] which is computed before
*/

void print_dp(vector<vector<pair<long, long>>>& dp) {
    cerr << "DP table : \n";
    for (long i = 0; i < dp.size(); i ++) {
        long j = 0;
        for (pair<long, long> score_rounds : dp[i]) {
            cerr << i << " " << j << " " << score_rounds.first << " " << score_rounds.second << "\n";
            j ++;
        }
    }
}

void testcase() {
    long n, m, k;
    long x;
    cin >> n >> m >> x >> k;

    vector<vector<pair<long, long>>> adj_list(n, vector<pair<long, long>>());
    vector<vector<pair<long, long>>> adj_list2(n, vector<pair<long, long>>());
    vector<vector<long>> dp(k + 1, vector<long>(n));
    
    // build adj list
    for (long i = 0; i < m; i ++) {
        long u, v, p;
        cin >> u >> v >> p;
        adj_list[u].push_back({v, p});
    }
    

    // fill dp
    // base case
    for (long i = 0; i < n; i ++) { 
        dp[0][i] = 0;
    }

    long stop = std::numeric_limits<long>::max();

    for (long i = 1; i < k + 1; i ++) {
        for (long j = 0; j < n; j ++) {

            long res_score = 0;

            // case where node is cul-de-sac
            if (adj_list[j].size() == 0) {
                dp[i][j] = dp[i][0];
            }

            else {
                for (pair<long, long> neighbor : adj_list[j]) {


                    long neighbor_score = dp[i - 1][neighbor.first];
                    res_score = max(res_score, neighbor_score + neighbor.second);
                }
                dp[i][j] = res_score;

                if (dp[i][j] >= x && j == 0) {
                    stop = min(stop, i);
                }
            }
        }
    }

    long final_score = dp[k][0];

    //print_dp(dp);

    // display result
    if (stop < std::numeric_limits<long>::max()) {
        cout << stop << "\n";
    }
    else if (final_score >= x) {
        cout << k << "\n";
    }
    else cout << "Impossible\n";
}

int main(long argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
