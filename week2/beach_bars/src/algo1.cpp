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
    std::sort(positions.begin(), positions.end());

    vector<pair<int, int>> limits(n, {-1, -1});
    limits[0] = {0, 0};
    limits[n-1] = {n - 1, n - 1};

    int i = 0; int j = 0;

    while (j < n - 1 && i < n - 1) {

        bool incr_i = false;
        bool incr_j =  false;

        if (positions[j] - positions[0] <= 100) { // bordure gauche
            limits[j].first = i;
            incr_j = true;
        }
        if (positions[n-1] - positions[i] <= 100) { // bordure droite
            limits[i].second = n - 1;
            limits[n - 1].first = min(limits[n - 1].first, i);
            if (!incr_j) incr_i = true;
            else {
                limits[j].second = n - 1;
            }
        }
        
        if (positions[j + 1] - positions[i] > 100) { // limite milieu
            limits[j].first = i;
            limits[i].second = j;
            incr_i = true;
        }
        else { // limite pas atteinte
            incr_j = true;
        } 

        if (incr_j) j ++;
        if (incr_i) i ++;
    }

    vector<int> save_largest_walks(n, std::numeric_limits<int>::max());
    int max_parasols = 0;
    int minimum_largest_walk = std::numeric_limits<int>::max();

    for (auto paire : limits) {
        max_parasols = max(max_parasols, paire.second - paire.first + 1);
    }

    for (int i = 0; i < n; i ++) {
        if (limits[i].second - limits[i].first + 1 == max_parasols) {
            int largest_walk = 0;
            for (int j = limits[i].first; j <= limits[i].second; j ++) {
                largest_walk = max(largest_walk, abs(positions[i] - positions[j]));
            }
            save_largest_walks[i] = largest_walk;
            minimum_largest_walk = min(minimum_largest_walk, largest_walk);
        }
    }

    cout << max_parasols << " " << minimum_largest_walk << "\n";
    for (int i = 0; i < n; i ++) {
        if (save_largest_walks[i] == minimum_largest_walk) {
            cout << positions[i] << " ";
        }
    }
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase ();
}
