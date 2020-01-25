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
https://www.geeksforgeeks.org/c-boostdynamic_bitset-class-with-examples/

interesting part with bitwise comparison and operators(&, <<). No need to convert to bitset to do bitwise comp.
(1 & 2) returns 0

Binary search: use of lowerbound : https://www.geeksforgeeks.org/lower_bound-in-cpp/
*/


void testcase(int n, int m) { // exponential sol, loop over all subsets 2^n

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

    // split and list
    int result = INT32_MAX;
    int S1 = n / 2;
    int S2 = n - S1;

    // for each subset of S1, compute #lights still needed for each room
    vector<pair<vector<int>, int>> needed_S1(1 << S1, {vector<int>(m), 0});

    for (long i = 0; i < 1 << S1; i ++) {

        for (int room = 0; room < m; room ++) {
            needed_S1[i].first[room] = targets[room];
        }
        
        for (int j = 0; j < S1; j ++) {

            if (1 << j & i) { // switch j is in subset i. Requires to be switched, count OFF
                
                for (int room = 0; room < m; room ++) {
                    needed_S1[i].first[room] -= switches[j][room].second;
                }
            }
            else { // switch j not switched, count ON contributions
                for (int room = 0; room < m; room ++) {
                    needed_S1[i].first[room] -= switches[j][room].first;
                }
            }
        }

        // count number of operations required
        boost::dynamic_bitset<> dyn_subset(S1, i);
        needed_S1[i].second = dyn_subset.count();
    }

    // for each subset of S2, compute #lights contributed for each room
    vector<pair<vector<int>, int>> contrib_S2(1 << S2, {vector<int>(m), 0});

    for (long i = 0; i < 1 << S2; i ++) {

        for (int room = 0; room < m; room ++) {
            contrib_S2[i].first[room] = 0;
        }
        
        for (int j = 0; j < S2; j ++) {

            if (1 << j & i) { // switch S1 + j is in subset i. Requires to be switched, count OFF
                
                for (int room = 0; room < m; room ++) {
                    contrib_S2[i].first[room] += switches[S1 + j][room].second;
                }
            }
            else { // switch S1 + j not switched, count ON contributions
                for (int room = 0; room < m; room ++) {
                    contrib_S2[i].first[room] += switches[S1 + j][room].first;
                }
            }
        }

        // count number of operations required
        boost::dynamic_bitset<> dyn_subset(S2, i);
        contrib_S2[i].second = dyn_subset.count();
    }

    // sort contribs of S2
    sort(contrib_S2.begin(), contrib_S2.end());
    vector<vector<int>> contrib_S2_vec(1 << S2);
    vector<int> contrib_S2_nops(1 << S2);
    for (auto it : contrib_S2) {
        contrib_S2_vec.push_back(it.first);
        contrib_S2_nops.push_back(it.second);
    }

    // for each subset S1 that has requirements for each room, use binary search to find if a subset in S2 has the right contribution
    for (auto paire_S1 : needed_S1) {

        auto cand_S2 = lower_bound(contrib_S2_vec.begin(), contrib_S2_vec.end(), paire_S1.first);
        if (*cand_S2 == paire_S1.first) {
            int idx = cand_S2 - contrib_S2_vec.begin();
            result = min(result, contrib_S2_nops[idx] + paire_S1.second);
        }        
    }

    if (result < INT32_MAX) cout << result << "\n";
    else cout << "impossible\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) {
        int n, m;   // for each subset of S1, compute #lights still needed for each room
        cin >> n >> m;
        testcase(n, m);
    }
}