#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <assert.h> 

using namespace std;

int aux(vector<int>& SherlockB, vector<int>& MoriartyB, int i, int n, vector<vector<int>> adj_list, int player) {
    
    // base condition
    if (i == n) return 0;
    int result;

    // recursive calls with neighbors of i if its Sherlocks turn
    if (player == 0) {

        if (SherlockB[i] >= 0) return SherlockB[i]; // already computed
        result = 0;
        for (int j=0; j<adj_list[i].size(); j++) {
            int neighbor = adj_list[i][j];
            int cand = aux(SherlockB, MoriartyB, neighbor, n, adj_list, 1);
            result = max(result, cand);
        }
        SherlockB[i] = result;
    }
    
    // recursive calls with neighbors of i if its Moriartys turn
    else {

        if (MoriartyB[i] >=0) return MoriartyB[i]; // already computed
        result = std::numeric_limits<int>::max();
        for (int j=0; j<adj_list[i].size(); j++) {
            int neighbor = adj_list[i][j];
            int cand = aux(SherlockB, MoriartyB, neighbor, n, adj_list, 0);
            result = min(result, cand);
        }
        MoriartyB[i] = result;
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

    // Compute Red path length (assumed unique)
    int len_red = 0;
    int pion = r;
    while(pion != n-1) {
        assert(adj_list[pion].size() == 1); 
        pion = adj_list[pion][0];
        len_red ++;
    }

    // Compute min and max of Black path with DP
    vector<int> SherlockB(n,-1);
    vector<int> MoriartyB(n,-1);

    int len_black = aux(SherlockB, MoriartyB, b, n, adj_list, 1);
    cout << len_red << " " << len_black << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) {
        testcase();
    }
}
