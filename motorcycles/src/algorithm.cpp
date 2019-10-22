#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;
typedef EK::Point_2 P;
typedef EK::Line_2 L;
typedef EK::Ray_2 R;
int INF = std::numeric_limits<int>::max();
using namespace std;


/*
idea for a linear algo:
Instead of computing, for each ray, the intersection with every other ray, we only compute the intersection with 4 rays:
- low-start is the ray with the lowest starting y coordinate
- up start ray is the ray with the biggest starting ray coordinate
- low rate is the ray with the smallest slope
- up rate is the ray with the biggest slope

The idea is to keep track of those 4 rays.
When looping over each ray, we examine its intersection with each of those 4 rays to see if a ray has to be discarded (set to 1 in active vector)
Then we check if this ray should be part of one of the 4 rays.

The solution is given by listing the active rays (0 in the vector)


NO DOES NOT WORK

linear idea in case of only upgoing motorcycles: sort starting points. set active lowest slope. iterate.
*/

void print_vector(vector<int> active) {
    for (int i=0; i<active.size(); i++) {
        cout << active[i] << " ";
    }
    cout << " \n";
}

void ride_forever() {

    long n; 
    cin >> n;
    vector<int> active(n, 0);
    vector<R> lines(n);

    // read direction of each biker
    long y0, x1, y1;
    for (int i=0; i<n; i++) {
        cin >> y0 >> x1 >> y1;
        lines[i] = R(P(0, y0), P(x1, y1));
    }

    // sort starting points

    

    // print active bikers
    for (int i=0; i<n; i++) {
        if (active[i] == 0) cout << i << " ";
    }
    cout << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while (t--) ride_forever();
}
