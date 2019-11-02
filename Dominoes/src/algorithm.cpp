#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;


int fall(int n, vector<int> a) {

    int max_reach = a[0];
    for (int i=0; i<n; i++) {
        if (i < max_reach && i+a[i] > max_reach) {
            if (i + a[i] >= n) {
                max_reach = n;
                break;
            }
            else max_reach = i + a[i];
        }
    }

    return max_reach;
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
        int result = fall(n, a);
        cout << result << endl;
    }

    return 0;
}

