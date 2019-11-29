#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>
#include <unordered_map>

/*
Tentative 2:
Not only sliding window, also binary search.
Instead of trying to fit a box for to each person at each round,
precompute the number of rounds. Then no need to actually allocate each box to a person.
Was not a good idea to iterate over m since no assumptions.
Dtermine minimum maximal number of boxes carried by 1 person just with weights and strengths

https://stackoverflow.com/questions/181693/what-are-the-complexity-guarantees-of-the-standard-containers
*/

using namespace std;

bool possible_in_k_rounds(int k, vector<int>& weights, vector<int>& strenghts) {

    int s = 0;
    int w = 0;
    // not a problem if some ppl cant lift anything: all that matters is that weights[m-1] gets assigned
    while (w < weights.size() && s < strenghts.size()) { // second condition: if all boxes not assigned with n ppl, would overflow and access random nb
    // would return true when actually false

        if (strenghts[s] >= weights[w]) {
            s ++;
            w += k; // s lifts boxes [s, s+k[
        }
        else {
            return false; // s cannot lift the heaviest box that falls to them
        }
    }
    //cout << s << " " << w << " " << k << " " << (w >= weights.size()) << "fdfa\n";
    return (w >= weights.size());
}

int min_rounds(vector<int>& weights, vector<int>& strenghts) {
    int left = 1;
    int right = weights.size();

    while (left < right) {
        int middle = (left + right) / 2;
        if (possible_in_k_rounds(middle, weights, strenghts)) {
            right = middle;
        }
        else {
            left = middle + 1;
        }
    }
    return left;
}

void testcase() {

    int n, m;
    cin >> n >> m;

    vector<int> strenghts(n);
    vector<int> weights(m);

    for(int i = 0; i < n; i++) {
        cin >> strenghts[i];
    }

    for (int i = 0; i < m; i++) {
        cin >> weights[i];
    }

    sort(strenghts.begin(), strenghts.end(), greater<int>());
    sort(weights.begin(), weights.end(), greater<int>());

    // check the work is doable and the heaviest box can be carried
    if (weights[0] > strenghts[0]) {
        cout << "impossible\n";
        return;
    }

    int min_max_rounds = min_rounds(weights, strenghts);
    int time = 3 * (min_max_rounds - 1) + 2;
    cout << time << "\n";
 }

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
