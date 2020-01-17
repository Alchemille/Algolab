#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

void testcase() {

    int n, d, a, b;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i ++) {
        cin >> v[i];
    }

    cin >> d >> a >> b;

    for (int i = 0; i < n; i ++) {
        if (i < d && (i < a || i > b)) {
            cout << v[i] << " ";
        }
        else if (i > d) {
            int ii = i - 1;
            if (ii < a || ii > b) {
                cout << v[i] << " ";
            }
        }
    }
    cout << "\n";
    cerr << d << " " << a << " " << b << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase() ;
}
