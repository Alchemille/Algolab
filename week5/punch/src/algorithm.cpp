#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <math.h>

using namespace std;

/*
First idea - thought that wanted exactly k liters
map (cost_per_liter -> pair(cost, volume)).
Like backpack problem, fill as much as possible with cheap beverages (respect limit k!)
To select among beverages of same cost_per_liter, solve subproblem:
Dispose of n objects of same values but different weights. Backpack capacity W.
Want to maximize weight first and then number of distinct object.

*/

void testcase() {

    int n, k;
    cin >> n >> k;
    vector<pair<int, int>> beverages(n);
    for (int i = 0; i < n; i ++) {
        int c, v;
        cin >> c >> v;
        beverages[i] = {c, v};
    }

    vector<vector<pair<int, int>>> memo(n, vector<pair<int, int>>(k+1, {-1, -1}));
    
    // base cases
    for (int i = 1; i <= k; i ++) {
        memo[0][i].first = std::ceil(float(i) / beverages[0].second) * beverages[0].first;
        memo[0][i].second = 1;
    }
    for (int i = 0; i < n; i ++) {
        memo[i][0] = {0, 0};
    }

    // induction
    for (int bev = 1; bev < n; bev ++) {
        for (int liters = 1; liters <= k; liters ++) {

            int volume_bottle = beverages[bev].second;
            int cost_bottle = beverages[bev].first;

            int cost_not_take = memo[bev - 1][liters].first;
            int distinc_not_take = memo[bev - 1][liters].second;
            int cost_take;
            int distinct_take;

            if (liters < volume_bottle) {
                cost_take = cost_bottle;
                distinct_take = 1;
            }
            else {
                int is_new_distinct = memo[bev][liters - volume_bottle] == memo[bev - 1][liters - volume_bottle];
                cost_take = memo[bev][liters - volume_bottle].first + cost_bottle;
                distinct_take = memo[bev][liters - volume_bottle].second + is_new_distinct;
            };

            if (cost_not_take == cost_take) {
                memo[bev][liters] = {cost_take, max(distinc_not_take, distinct_take)};
            }
            else if (cost_take < cost_not_take) {
                memo[bev][liters] = {cost_take, distinct_take};
            }
            else memo[bev][liters] = {cost_not_take, distinc_not_take};
        }
    }

    // output result
    cout << memo[n-1][k].first << " " << memo[n-1][k].second << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) {
        testcase();
    }
}
