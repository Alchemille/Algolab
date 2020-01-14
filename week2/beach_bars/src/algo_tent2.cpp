#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

/*
Thought that bar could only be locate where there is a parasol
Hence solved the wrong pb...
used 2 pointeurs i and j. For each parasol, stored a paire limit to indicate 
leftmost and rightmost achievable parasols
*/

void testcase() {

    int n;
    cin >> n;
    vector<int> positions(n);
    
    for (int i = 0; i < n; i ++) {
        cin >> positions[i];
    }

    // sort and normalize positions
    std::sort(positions.begin(), positions.end());
    long min_position = *(positions.begin());
    for (int i = 0; i < n; i ++) {
        positions[i] -= min_position;
    }
    long n_candidates = *(positions.end() - 1) + 1;

    // for each candidate bar position, count parasols up until the position
    vector<int> n_parasols_before(n_candidates);
    int parasol_counter = 0;
    for (long cand = 0; cand < n_candidates; cand ++) {
        if (cand == positions[parasol_counter]) {
            n_parasols_before[cand] = ++parasol_counter; // important to increment before setting n_parasols_before
        }
    }

    // for each candidate bar position, use the known leftmost and rightmost limits to count parasols reachable
    vector<int> vec_n_parasols(n_candidates);
    int max_parasols = 0;
    for (long cand = 0; cand < n_candidates; cand ++) {
        int leftmost = max(cand - 100, long(0));
        int rightmost = min(cand + 100, n_candidates - 1);
        int n_parasols = n_parasols_before[rightmost] - n_parasols_before[leftmost];
        if (leftmost == 0 || n_parasols_before[leftmost] != n_parasols_before[leftmost - 1]) { // order of boolean expressions important to avoid segfault
            n_parasols ++;
        }
        vec_n_parasols[cand] = n_parasols;
        max_parasols = max(max_parasols, n_parasols);
    }

        

    // vector<int> save_largest_walks(n, std::numeric_limits<int>::max());
    // int minimum_largest_walk = std::numeric_limits<int>::max();

    // cout << max_parasols << " " << minimum_largest_walk << "\n";
    // for (int i = 0; i < n; i ++) {
    //     if (save_largest_walks[i] == minimum_largest_walk) {
    //         cout << positions[i] << " ";
    //     }
    // }
    // cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase ();
}
