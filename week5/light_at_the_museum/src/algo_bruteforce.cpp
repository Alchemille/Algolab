#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/dynamic_bitset.hpp> 
#include <iostream> 

using namespace std;

/*
pieges:
INT64_MAX is actually a long
careful when DP on value and not index, allocate enough space!
boost_bistset.to_u_long() converts back to int
boost_bitset.count() to count how many set to 1
careful with operator 1 << decalage. eg 1 << 0 = 1
dynamic_bitset and 1 << n have very different purposes!

interesting part with bitwise comparison and operators(&, <<). No need to convert to bitset to do bitwise comp.
*/

void sol_1_room(int n, int m) { // simple DP

    int target; // only 1 room!
    cin >> target;
    vector<pair<int, int>> switches(n);
    vector<vector<int>> memo(n, vector<int>(target + 1, std::numeric_limits<int>::max()));

    for (int i = 0; i < n; i ++) {
        cin >> switches[i].first >> switches[i].second;
    }

    // base cases
    for (int j = 0; j <= target; j ++) {

        bool turn_on = switches[0].first == j;
        bool turn_off = switches[0].second == j;

        if (turn_on) memo[0][j] = 0;
        else if (turn_off) memo[0][j] = 1;
    }

    // induction
    for (int i = 1; i < n; i ++) {
        for (int j = 0; j <= target; j ++) {

            int bulbs_on = switches[i].first;
            int bulbs_off = switches[i].second;

            int turn_on = (j >= bulbs_on ? memo[i - 1][j - bulbs_on] < std::numeric_limits<int>::max() : false);
            int turn_off = (j >= bulbs_off ? memo[i - 1][j - bulbs_off] < std::numeric_limits<int>::max() : false);

            if (turn_on) memo[i][j] = min(memo[i][j], memo[i - 1][j - bulbs_on]);
            if (turn_off) memo[i][j] = min(memo[i][j], memo[i - 1][j - bulbs_off] + 1);
        }
    }

    // cout result
    if (memo[n - 1][target] < std::numeric_limits<int>::max()) {
        cout << memo[n - 1][target] << "\n";
    }
    else cout << "impossible\n";
}

void sol_exp(int n, int m) { // exponential sol, loop over all subsets 2^n

    vector<int> targets(m);
    vector<vector<pair<int, int>>> switches(n, vector<pair<int, int>>(m));

    // read inputs
    for (int i = 0; i < m; i ++) {
        cin >> targets[i];
    }
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            cin >> switches[i][j].first >> switches[i][j].second;
        }
    }

    // bruteforce
    int result = INT32_MAX;
    for (int subset = 0; subset < 1 << n; subset ++) { // for each of 2^n subsets, the switches at 1 are switched

        vector<int> sums(m, 0);
        boost::dynamic_bitset<> dyn_subset(n, subset);

        for (int sw = 0; sw < n; sw ++) {
            
            boost::dynamic_bitset<> dyn_sw(n, sw);

            if (1 << sw & subset) { // bitwise comparison. true if sw is part of subset of switches that are switched

                // add contributions of sw to sums. 
                for (int room = 0; room < m; room ++) {
                    sums[room] += switches[sw][room].second;
                }
            }

            else { // sw not part of subset. add its on contributions
                for (int room = 0; room < m; room ++) {
                    sums[room] += switches[sw][room].first;
                }
            }
        }

        // check if targets satisfied for each room
        bool ok = true;
        for (int room = 0; room < m; room ++) {
            if (sums[room] != targets[room]) {
                ok = false;
                break;
            }
        }

        int number_operations = dyn_subset.count();
        if (ok) result = min(number_operations, result);
    }

    if (result < INT32_MAX) cout << result << "\n";
    else cout << "impossible\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) {
        int n, m;
        cin >> n >> m;
        if (m == 1) {
            sol_1_room(n, m);
        }
        else {
            sol_exp(n, m);
        }
    }
    cerr << (1 & 2) << "\n";
}
