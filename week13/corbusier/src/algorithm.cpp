#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

/*
first idea (too slow) :
Parcourir all the possible hauteurs that is multiple of k H = k * p, but too many choices (since height could be (h * n)/k aka 10^9 * 10^3)
For each hauteur that is a multiple of k, check if there is a partial sum that sums to H (linear)

second idea: O(n * k)
greedy classes de congruences. 
In total, 2^n partial sums (power set). However each partial sum falls into one out of k classes (remainder)
Remember that if h % k = r1 and h' % k = r1' then h + h' % k = r1 + r1' 
For each adder height, at most k remainders to update
Loop over heights then remainders filled already.

Trick:
for each new height, important to update the remainders after they are all computed!

*/

void testcase() {

    int n, i, k, h;
    cin >> n >> i >> k;
    vector<bool> remainders(k, false);

    for (int u = 0; u < n; u ++) {

        cin >> h;
        int r1 = h % k;
        vector<bool> update_remainders(k, false);
        update_remainders[r1] = true;

        for (int r2 = 0; r2 < k; r2 ++) {
            if (remainders[r2]) {
                update_remainders[(r1 + r2) % k] = true;
            }
        }

        for (int r = 0; r < k; r ++) {
            if (update_remainders[r]) {
                remainders[r] = true;
            }
        }
    }

    if (remainders[i]) cout << "yes\n";
    else cout << "no\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
