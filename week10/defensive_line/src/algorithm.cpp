#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

/*
First idea:
Only sliding window, in O(m):
Comput attackable intervals with 2 pointers i and j
For each interval, record the number of defendants in a vector.
If the vector has < m elements, fail
Sort this vector in reverse order.
Select m left elems and return the sum of number of defendants for those m selected intervals.

BUT hadnt seen that this could happen:
n = 5, m = 1, k = 5,
A = [4, 1, 1, 1, 1, 1]
The algo above returns [4, 1] -> 3
But [1, 1, 1, 1, 1] -> 5 is better

SO need to use sliding window several times to record all possible attackable itervals,
not just the left disjoint ones discovered (approach was greedy)

DP
- Tried linear in m (comments below): but was computing max defendants in disjoint intervals without number of attackants constraint
- To account for the limit of attackers, sqaure DP in n.m
T[i, j], i in [0, n-1], j in [0, m-1]
is the maximal number of defendants in j disjoint intervals within defendents [0, i]. 
If not j disjoint attackable intervals in [0, i], return fail
Result is thus T[n-1, m-1]

be careful about:
- whitespaces
*/

void testcase() {
    int n, m, k;
    cin >> n >> m >> k;

    vector<int> defense(n);
    for (int i = 0; i < n; i++) {
        cin >> defense[i];
    }

    if (n < 2) {
        cout << "fail" << "\n";
        return;
    }

    // sliding window;
    vector<pair<int, int>> attackable_int;
    vector<int> interval_strenght;
    int i = 0; int j = 0; int sum = defense[0];

    while (j < n) {
        if (sum < k) {
            //cout << "1\n";
            j ++;
            if (j < n) sum += defense[j];
        }
        else if (sum > k && i == j) {
            //cout << "2\n";
            i ++;
            j ++;
            if (i < n) sum = defense[i];
        }
        else if (sum > k) {
            //cout << "3\n";
            sum -= defense[i];
            i ++;
        }
        else if (sum == k) {
            //cout << "4\n";
            attackable_int.push_back({i, j});
            interval_strenght.push_back(j - i + 1);
            sum -= defense[i];
            i ++;
            j ++;
            if (j < n) sum += defense[j];            
        }
    }
    // DP part
    sort(attackable_int.begin(), attackable_int.end()); // notice that first1 =/= first 2 and second1 =/= second2 for all pairs. Moreover, first1 < first2 <=> second1 < second2. No overlap.
    vector<vector<int>> T(n, vector<int>(m, 0)); // T[right, att] is the max num of deffenders attacked with defenders in [0, right] and att attackers

    if (attackable_int.size() == 0) {
        cout << "fail\n";
        return;
    }

    int a = attackable_int[0].first;
    int b = attackable_int[0].second;

    // *** BELOW : tried to do linear DP ***
    // *** best_score[n - 1] is the maximal number of defendants in disjoint intervals, does not take into account number of attackers ***
    // vector<int> best_score(n, 0);
    // best_score[b] = b - a + 1;
    // int pointer_intervals = 1;
    // for (int right = b + 1; right < n; right ++) {
    //     if (attackable_int[pointer_intervals].second == right) {
            
    //         int left = attackable_int[pointer_intervals].first;
    //         int choose_def = right - left + 1 + best_score[right - 1];
    //         int not_choose_def = best_score[left - 1];

    //         best_score[right] = max(choose_def, not_choose_def);
        
    //         pointer_intervals ++;
    //     }
    //     else {
    //         best_score[right] = best_score[right - 1];
    //     }
    // }


    // square DP

    // first interval and all attackers
    for (int i = 0; i < m; i++) {
        T[b][i] = b - a + 1;
    }

    // all intervals and 1 attacker
    int pointer_intervals = 1;
    int right = b + 1;

    while (right < n) {

        if (pointer_intervals < attackable_int.size() && attackable_int[pointer_intervals].second == right) { // an attackable interval ends at right

            // see if this interval can contribute to a better T[right, att]
            int left = attackable_int[pointer_intervals].first;
            int choose_def = right - left + 1;
            int not_choose_def = T[right - 1][0];

            T[right][0] = max(choose_def, not_choose_def);
            pointer_intervals ++;
        }
        else {
            T[right][0] = T[right - 1][0];
        }

        right ++;
    }

    // recurrence
    for (int att = 1; att < m; att ++) {

        pointer_intervals = 1;
        int attacks_done = 0;

        for (int right = b + 1; right < n; right ++) { // start iteration at the end of the first interval

            if (pointer_intervals < attackable_int.size() && attackable_int[pointer_intervals].second == right) { // an attackable interval ends at right

                // see if this interval can contribute to a better T[right, att]
                int left = attackable_int[pointer_intervals].first;
                int choose_def = right - left + 1 + T[left - 1][att - 1]; // key: only 1 new interval is accounted for each att
                int not_choose_def = T[right - 1][att];
                
                if (choose_def > not_choose_def) {
                    T[right][att] = choose_def;
                }
                else T[right][att] = not_choose_def;

                if (T[right][att] > T[right][att - 1]) { // att + 1 disjoint intervals were used
                    attacks_done ++;
                }
                pointer_intervals ++;
            }

            else { // right is not the end of an interval
                T[right][att] = T[right - 1][att]; 
            }
        }

        if (attacks_done == 0) { // not possible to have att disjoint intervals
            cout << "fail\n";
            return;
        }
    }

    int result = T[n-1][m-1];
    cout << result << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
