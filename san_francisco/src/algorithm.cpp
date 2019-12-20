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
DP[i, j] is a pair (score, steps) a player can get with i remaining turns and starting from node j.
The DP is solved top-down.
A 2-steps adj-list is built to know which edge a player can get in her next turn.
The recurrence is basically
DP[i + 1, j] = max_over_2_steps_neighbors_n (DP(i, n))
If this value is bigger than m, the number of steps required to get it is returned too.
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
    vector<vector<pair<long, long>>> dp(k + 1, vector<pair<long, long>>(n, {-1, -1}));
    
    // build adj list
    for (long i = 0; i < m; i ++) {
        long u, v, p;
        cin >> u >> v >> p;
        adj_list[u].push_back({v, p});
    }
    

    // fill dp
    // base case
    for (long i = 0; i < n; i ++) { 
        dp[0][i] = {0, 0};
    }

    long stop = std::numeric_limits<long>::max();
    long stop_value = 0;

    for (long i = 1; i < k + 1; i ++) {
        for (long j = 0; j < n; j ++) {

            long res_score = 0;
            long res_rounds = i;
            //cout << "recursive call " << rounds_left << " " << node << "\n";

            // case where node is cul-de-sac
            if (adj_list[j].size() == 0) {
                dp[i][j] = dp[i][0];
            }

            else {
                for (pair<long, long> neighbor : adj_list[j]) {


                    long neighbor_score = dp[i - 1][neighbor.first].first;
                    long neighbor_rounds = dp[i - 1][neighbor.first].second;

                    res_score = max(res_score, neighbor_score + neighbor.second);
                }
                dp[i][j] = {res_score, res_rounds};

                if (dp[i][j].first >= x && j == 0) {
                    stop = min (stop, i);
                    stop_value = dp[i][j].first;
                }
            }
        }
    }

    long final_score = dp[k][0].first;
    long final_rounds = dp[k][0].second;

    //print_dp(dp);

    // display result
    cerr << stop << " " << stop_value << " " << final_score << " " << final_rounds << x << "\n";
    if (stop < std::numeric_limits<long>::max()) {
        cout << stop << "\n";
    }
    else if (final_score >= x) {
        cout << final_rounds << "\n";
    }
    else cout << "Impossible\n";
}

int main(long argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
