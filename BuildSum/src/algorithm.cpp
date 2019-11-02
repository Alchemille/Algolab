#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;


int build_sum(int n, vector<int> a) {
    int res = 0;
    for (int i=0; i<n; i++) {
        res += a[i];
    }
    return res;
}


int main() {

    int t;
    cin >> t;
    while(t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i=0; i<n; i++) {
            cin >> a[i];
        }
        int result = build_sum(n, a);
        cout << result << endl;
    }

    return 0;
}
