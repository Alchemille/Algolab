#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EK;
typedef CGAL::Delaunay_triangulation_2<EK> Triangulation;
typedef EK::Point_2 P;

/*
Project lamp disk over scene. radius is h.

First idea:
Use exact type. Compute persons disks and lamps disks, and lookd for intersections.
But slow.

Now:
Simply evaluate distance in double for loop. 
If notice there are survivors, the winners must be survivors. Can stop the double for loop
and use triangulation and nearest_vertex to just test if survives.
*/

using namespace std;

void testcase() {

    int m, n;
    cin >> m >> n;

    vector<pair<P, int>> persons(m);
    vector<P> lamps(n);

    for (int i = 0; i < m; i++) {
        int x, y, r;
        cin >> x >> y >> r;
        persons[i] = {P(x, y), r};
    }

    int h;
    cin >> h;
    EK::FT r_lamps = h;

    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        lamps[i] = P(x, y);
    }    

    // Triangulation to speed up if we know there are survivors
    Triangulation t;
    t.insert(lamps.begin(), lamps.end());

    // iterate over people and lamps to search for intersections
    // everyone may get kill, so need to record dying times and latest die
    vector<int> dying_time(m, n);
    int latest_dead = 0;

    for (int i = 0; i < m; i++) {

        EK::FT max_d = persons[i].second + r_lamps;

        if (latest_dead != n) { // for now, no one survives to all lights
            for (int j = 0; j < n; j++) {
                EK::FT d = CGAL::squared_distance(persons[i].first, lamps[j]);
                if (d < max_d * max_d) {
                    dying_time[i] = j;
                    latest_dead = max(latest_dead, j);
                    break;                
                }
            }

            // survived to all lamps
            if (dying_time[i] == n) {
                latest_dead = n;
            }
        }
        else {
            P n = t.nearest_vertex(persons[i].first)->point();
            EK::FT d = CGAL::squared_distance(n, persons[i].first);
            if (d < max_d * max_d) {
                dying_time[i] = 0; // dont care when died, just that did not survive
            }
        }

    }

    // output result
    for (int i = 0; i < m; i++) {
        if (dying_time[i] == latest_dead) {
            cout << i << " ";
        }
    }
    cout << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
