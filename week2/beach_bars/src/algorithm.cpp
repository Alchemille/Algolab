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

My Sol is quite close in the end and in O(n)
- compute parasol pairs (i, j) such that the number of parasols in between is maximal.
- use sliding window to do so.
- the need to select result_bars that minimize the largest walk.
- simply loop over the pairs and look at the distance between i and j  to do so.
- when have a pair that creates a result_bar, put bar location in the middle, and be carefull whether odd or even 
- if distance between i and j odd, will have 2 middle bars to output for this pair
*/

void testcase() {

    int n;
    cin >> n;
    vector<int> positions(n);
    
    for (int i = 0; i < n; i ++) {
        cin >> positions[i];
    }
    
    std::sort(positions.begin(), positions.end());
    int max_parasols = 0;
    vector<pair<int, int>> limits;
    int i = 0; int j = 0; // 2 pointers

    while (j < n - 1) {

        bool incr_i = false;
        bool incr_j =  false;

        if (positions[j] - positions[i] <= 200) {
            if (max_parasols < j - i + 1) {
                max_parasols = j - i + 1;
                limits = vector<pair<int, int>>{{i, j}}; // notice double curly brackets
            }
            else if (max_parasols == j - i + 1) {
                limits.push_back({i, j});
            }
        }

        if (positions[n-1] - positions[i] <= 200) { // bordure droite

            if (max_parasols < n - 1 - i + 1) {
                max_parasols = n - 1 - i + 1;
                limits = vector<pair<int, int>>{{i, n-1}}; // notice double curly brackets
            }
            else if (max_parasols == n - 1 - i + 1) {
                limits.push_back({i, n - 1});
            }
            break;
        }

        if (positions[j + 1] - positions[i] > 200) { // limite milieu
            incr_i = true;
            if (i == j) incr_j = true;
        }
        else { // limite pas atteinte   
            incr_j = true;
        } 

        if (incr_j) j ++;
        if (incr_i) i ++;
    }

    vector<long> resulting_bars;
    int minimum_largest_walk = std::numeric_limits<int>::max();
    int ecart;
    bool odd;

    for (auto paire : limits) {

        if ((positions[paire.second] - positions[paire.first]) % 2 == 1) { // careful, + and - prioritaires over %
            ecart = (positions[paire.second] - positions[paire.first]) / 2 + 1;
            odd = true;
        }
        else {
            ecart = (positions[paire.second] - positions[paire.first]) / 2;
            odd = false;
        }

        int middle = (positions[paire.first] + positions[paire.second]) / 2;

        if (ecart < minimum_largest_walk) {
            minimum_largest_walk = ecart;
            resulting_bars = vector<long>{middle}; // node how to init vector with single value and {}
            if (odd) resulting_bars.push_back(middle + middle / abs(middle));
        }
        else if (ecart == minimum_largest_walk) {
            resulting_bars.push_back(middle);
            if (odd) resulting_bars.push_back(middle + middle / abs(middle));
        }
    }

    cout << max_parasols << " " << minimum_largest_walk << "\n";
    sort(resulting_bars.begin(), resulting_bars.end());
    for (int i : resulting_bars) {
        cout << i << " ";
    }
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase ();
}