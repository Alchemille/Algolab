#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <assert.h> 
#include <iostream>
#include <chrono>

using namespace std;

/*
Awesome, found solution right away for case with unique Red path.
Compared the unique red path lenght with the black path len that Sherlock maximizes and Sherlock minimizes and begins.
This black path len is computed with DP. Both S and M impact it.
n subproblems. e total computations (somme neighbors) for all subproblems, hence O(n+m)

Generalisation:
No change for black path.
For red path, inverse roles but same fonction: Sherlock minimizes and begins, Moriarty maximizes.

*/

int aux(vector<int>& maxPlayer, vector<int>& minPlayer, int i, int n, vector<vector<int>>& adj_list, int player) {
    
    // base condition
    if (i == n) return 0;
    int result;

    // If player 0 aka maxPlayer, recursive calls with neighbors of i
    if (player == 0) {

        if (maxPlayer[i] >= 0) return maxPlayer[i]; // already computed
        result = 0;
        for (int j=0; j<adj_list[i].size(); j++) {
            int neighbor = adj_list[i][j];
            int cand = 1 + aux(maxPlayer, minPlayer, neighbor, n, adj_list, 1);
            result = max(result, cand);
        }
        //cerr << "S " << i << " " << n << " " << result << "\n";
        maxPlayer[i] = result;
    }
    
    
    // If player 1 aka minPlayer, recursive calls with neighbors of i
    else {

        if (minPlayer[i] >=0) return minPlayer[i]; // already computed
        result = std::numeric_limits<int>::max();
        for (int j=0; j<adj_list[i].size(); j++) {
            int neighbor = adj_list[i][j];
            int cand = 1 + aux(maxPlayer, minPlayer, neighbor, n, adj_list, 0);
            result = min(result, cand);
        }
        //cerr << "M " << i << " " << n << " " << result << "\n";
        minPlayer[i] = result;
    }

    return result;

} 


void testcase() {

    // read inputs and redinex vertices in [0, n-1]
    int n, m, r, b, u, v;
    cin >> n >> m >> r >> b;
    r --; b--;
    vector<vector<int>> adj_list(n, vector<int>());

    for (int i=0; i<m; i++) {
        cin >> u >> v;
        adj_list[u-1].push_back(v-1);
    }

    // Compute min and max of Black path with DP. Same for Red path
    vector<int> SherlockB(n,-1);
    vector<int> MoriartyB(n,-1);
    vector<int> SherlockR(n,-1);
    vector<int> MoriartyR(n,-1);

    int len_red = aux(MoriartyR, SherlockR, r, n-1, adj_list, 1); // Sherlock begins to play hence player 1
    int len_black = aux(SherlockB, MoriartyB, b, n-1, adj_list, 1); // Moriarty begins to play hence player 1
    
    //cerr << len_red << " " << len_black << "\n";
    if (len_red == len_black && len_black % 2 == 1) cout << "0\n";
    else if (len_red == len_black && len_black % 2 == 0) cout << "1\n";
    else if (len_red < len_black) cout << "0\n";
    else cout << "1\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) {
        testcase();
    }
}
