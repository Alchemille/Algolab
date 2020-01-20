#include <stdlib.h>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

#define MUST_COVER 0
#define MUST_TAKE 1
#define ALREADY_COVERED 2

int dp(vector<vector<int>> &t, vector<int> &c, vector<vector<long>> &memo, int curr, int state){
    if(memo[state][curr] != -1)
        return memo[state][curr];
    
    int take = c[curr];
    for(int i = 0; i < t[curr].size(); i++){
        take += dp(t, c, memo, t[curr][i], ALREADY_COVERED);
    }
    
    if(state == MUST_TAKE){ // Should take in any case
        return memo[state][curr] = take;
    }

    // Decide which child is better to take
    int take_childs = 0;
    for(int i = 0; i < t[curr].size(); i++){
        take_childs += dp(t, c, memo, t[curr][i], MUST_COVER);
    }
    
    int res = 0;
    int do_not_take = INT_MAX;
    for(int i = 0; i < t[curr].size(); i++){ // take at least one child
        res = take_childs - dp(t, c, memo, t[curr][i], MUST_COVER) + dp(t, c, memo, t[curr][i], MUST_TAKE);  
        if(res < do_not_take)
            do_not_take = res;      
    }

    if(state == ALREADY_COVERED) // We are already covered is better to take a child or not?
        if(take_childs < do_not_take)
            do_not_take = take_childs;

    return memo[state][curr] = min(take, do_not_take);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        int n;
        cin >> n;

        vector<vector<int>> tree(n);
        vector<int> cost(n);
        for(int i = 0; i < n - 1; i++){
            int a, b; 
            cin >> a >> b;
            tree[a].push_back(b);
        }

        for(int i = 0; i < n; i++)
            cin >> cost[i];
        
        vector<vector<long>> memo(3, vector<long>(n, -1));
        int res = dp(tree, cost, memo, 0, MUST_COVER);

        cout << res << "\n";

    }
    return 0;
}
    