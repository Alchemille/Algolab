#include <iostream>
#include <vector>
using namespace std;

unsigned nChoosek(int n, int k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

int number_even_pairs(int n, vector<int> a) {

    // compute partial sums
    vector<int> s(n, 0);
    s[0] = a[0];
    for (int i=1; i<n; i++) {
        s[i] = s[i-1] + a[i];
    }

    // compute E and O
    int E = 0;
    int O = 0;
    for (int i=0; i<n; i++){
        if (s[i] % 2 == 0) E++;
        else O++;
    }

    return nChoosek(E, 2) + nChoosek(O, 2) + E;
}

int without_combi(int n, vector<int> x) {

	vector<int> even(n); // number of even sums that end in i
	vector<int> odd(n); // number of odd sums that end in i
	int n_pairs = 0;

	for (int i = 0; i < n; i++) {
		if (i == 0) {
			if (x[i] == 0) { even[i] = 1; }
			else { odd[i] = 1; }
		} else {
			if (x[i] == 0) {
				even[i] = even[i - 1] + 1;
				odd[i] = odd[i - 1];
			} else {
				even[i] = odd[i - 1];
				odd[i] = even[i - 1] + 1;
			}
		}
		n_pairs += even[i];
	}

	return n_pairs;
}

int main() {
    int t;
    int n;
    cin >> t;

    while(t--) {
        cin >> n;
        vector<int> a(n);
        for(int i=0; i<n; i++) {
            cin >> a[i];
        }

        int result = without_combi(n, a);
        //int result = number_even_pairs(n, a);
        std::cout << result << std::endl;
    }
    return 0;
}
