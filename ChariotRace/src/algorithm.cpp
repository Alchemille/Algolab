#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>


/*
sol.cpp is a very close recurrence, also interesting. both are equivalent.
Partitions as 
- must cover (could take or not)
- must take
- already covered (could take or not)

while I do
- already_covered_not_take
- take
- must cover not take

Point de tension:
- In case 2, said that all children should be taken
- actually, at least one child!
- for each child, force to take it (state 1). Then for each other child, give choice (state 1 or 2)
- avoid double for loop!! by reusing memo[node][0] that sums all children when they all have choice.
- loop over the child that has to be taken, remove its maybe value, add its sure value

*/

#define ALREADY_COVERED_NOT_TAKE 0;
#define TAKE 1;
#define NOT_TAKE_TO_COVER 2;

using namespace std;

int dp(vector<vector<int>>& adj_list, vector<int>& costs, vector<vector<int>>& memo,
    int node, int state) {

    if (memo[state][node] != -1) return memo[state][node];

    if (state == 0) {

        // base case
        if (adj_list[node].size() == 0) {
            memo[state][node] = 0;
        }
        else {
            int result = 0;
            for (int v : adj_list[node]) {
                result += min(dp(adj_list, costs, memo, v, 1), dp(adj_list, costs, memo, v, 2));
            }  
            memo[state][node] = result;   
        }
    }

    else if (state == 1) {
        // base case
        if (adj_list[node].size() == 0) {
            memo[state][node] = costs[node];
        }
        else {
            int result = costs[node];
            for (int v : adj_list[node]) {
                result += min(dp(adj_list, costs, memo, v, 1), dp(adj_list, costs, memo, v, 0));
            }  
            memo[state][node] = result;   
        }
    }

    else if (state == 2) { // at leat one child must be taken
        // base case
        if (adj_list[node].size() == 0) {
            memo[state][node] = costs[node]; // if node has no child, must be taken anyway
        }
        else {

            int result = std::numeric_limits<int>::max();
            int all_children_maybe = dp(adj_list, costs, memo, node, 0);

            for (int v : adj_list[node]) {

                int this_child_sure = dp(adj_list, costs, memo, v, 1);
                int this_child_maybe = min(dp(adj_list, costs, memo, v, 1), dp(adj_list, costs, memo, v, 2));
                int result_this_child_sure = all_children_maybe - this_child_maybe + this_child_sure;

                result = min(result, result_this_child_sure);
            }
            memo[state][node] = result;
        }
    }

    return memo[state][node];

}

void testcase() {
    int n, u, v; 
    cin >> n;
    vector<vector<int>> adj_list(n);
    vector<int> costs(n);

    for (int i = 0; i < n-1; i ++) {
        cin >> u >> v;
        adj_list[u].push_back(v);
    }
    for (int i = 0; i < n; i ++) {
        cin >> costs[i];
    }

    // launch DP
    vector<vector<int>> memo(3, vector<int>(n, -1));
    
    int choice1 = dp(adj_list, costs, memo, 0, 2);
    int choice2 = dp(adj_list, costs, memo, 0, 1);
    int result = min(choice1, choice2);

    // // debug
    // for (int i = 0; i < 3; i ++) {
    //     for (int j = 0; j < n; j ++) {
    //         cout << memo[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    //cout << choice1 << " " << choice2 << "\n";

    cout << result << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
