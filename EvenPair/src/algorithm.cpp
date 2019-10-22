#include <iostream>
using namespace std;

unsigned nChoosek(unsigned n, unsigned k)
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

int number_even_pairs(int n, vector& a) {

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

    std::cout << "hi" << std::endl;
    return nChoosek(2, E) + nChoosek(2, O) + E;
}

int main() {

    int n;
    cin >> n;
    vector<int> a(n);
    for(int i=0; i<n; i++) {
        cin >> a[i]
    }

    std::cout << "test" << std::endl;
    return number_even_pairs(n, a);

}
