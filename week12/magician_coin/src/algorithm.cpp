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

*/

int aux(vector<vector<int>>& dp, int round, int node, int max_rounds, vector<vector<pair<int, int>>> adj_list2) {

    if (round == 0) {
        return 0;
    }

    int res = 0;
    for (pair<int, int> neighbor2 : adj_list2[node]) {
        res = max(res, neighbor2.second + aux(dp, round + 1, neighbor2.first, max_rounds, adj_list2));
    }
    dp[round][node] = res;
    return res;
}

void testcase() {
    int n, k, m;
    cin >> n >> k >> m;

    vector<vector<pair<int, int>>> adj_list(n, vector<pair<int, int>>());
    vector<vector<pair<int, int>>> adj_list2(n, vector<pair<int, int>>());
    vector<vector<int>> dp(k, vector<int>());
    
    // build adj list
    for (int i = 0; i < m; i ++) {
        int u, v, p;
        cin >> u >> v >> p;
        adj_list[u].push_back({v, p});
    }
    
    // build 2 steps adj list
    for (int i = 0; i < n; i ++) {
        for (pair<int, int> pair1 : adj_list[i]) {
            int neighbor = pair1.first;
            for (pair<int, int> pair2 : adj_list[neighbor]) {
                int neighbor2 = pair2.first;
                adj_list2[i].push_back({neighbor2, pair1.second});
            }
        }
    }

    // lauch DP
    int result = aux(dp, 0, 0, k, adj_list2);
    for (pair<int, int> neighbors1 : adj_list[0]) {
        result = max(result, aux(dp, 0, neighbors1.first, k, adj_list2));
    }

    // display result
    if (result >= x) {
        return 
    }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
