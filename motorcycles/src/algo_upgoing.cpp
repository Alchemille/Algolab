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
typedef EK::Direction_2 D;

int INF = std::numeric_limits<int>::max();
using namespace std;


/*
Idea for a linear algo.
ONLY IN CASE OF UPGOING MOTOS

The idea is to sort motos according to the slope rate in an array, and according to the starting point (y0) in another.
We take the moto with the smallest slope and add it to list of forever going motos. 
All motos with a smaller necessarily die.
Then iterate, looking for the next moto that has the smallest slope and is above to the previous smallest slope moto.
Until the moto with highest slope is checked.

Problem: 
I try to sort motoes according to different criteria in different arrays. 
First motos are sorted according to starting point so that the member start_index of each moto can be set.
Then motoes are sorted according to slope so that the member slope_index can be set.
However, I noticed that the indices members are messed up.
After being set in the first sorting, the start_index seems not to have been changed when I do the second sorting.
So maybe the setting of start_date was done on a copy, but I dont see how its possible

Sort stucture according to 1 member value
https://stackoverflow.com/questions/873715/c-sort-with-structs
http://www.cplusplus.com/forum/general/97555/
https://stackoverflow.com/questions/12662891/how-can-i-pass-a-member-function-where-a-free-function-is-expected
https://stackoverflow.com/questions/50549611/slicing-a-vector-in-c
https://stackoverflow.com/questions/11348376/std-vector-c-deep-or-shallow-copy
*/

struct motorcycle {
    long start;
    EK::RT slope;
    int start_index;
    int slope_index;
    int input_index;
    R rayon;
    bool active;
};

bool sort_start(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    return m1.start < m2.start;
}

bool sort_slope(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    if (m1.slope != m2.slope) return m1.slope < m2.slope;
    else return m1.start < m2.start;
}

bool sort_input(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    return m1.input_index < m2.input_index;
}

void ride_forever() {

    long n; 
    cin >> n;
    vector<motorcycle> input_motos;
    vector<motorcycle> slope_motos;

    // read direction of each biker
    long y0, x1, y1;
    EK::RT slope;
    D direction;
    R rayon;

    for (int i=0; i<n; i++) {
        cin >> y0 >> x1 >> y1;
        rayon = R(P(0, y0), P(x1, y1));
        slope = rayon.direction().dy() / rayon.direction().dx();
        input_motos.push_back({y0, slope, i, i, i, rayon, false});
    }

    // sort starting points
    std::sort(input_motos.begin(), input_motos.end(), sort_start);
    for (int i=0; i<n; i++) {
        motorcycle* m = &input_motos[i]; // if no pointer, will make copy -> non persistent change
        (*m).start_index = i; 
        //cout << "sort start " << " " << " " << (*m).input_index << " " << (*m).start_index << "\n";
    }
    
    // sort slopes
    std::sort(input_motos.begin(), input_motos.end(), sort_slope);
    for (int i=0; i<n; i++) {
        motorcycle* m = &input_motos[i]; // if no pointer, will make copy -> non persistent change
        (*m).slope_index = i;
        //cout << "sort slope " << " " << (*m).slope << " " << (*m).input_index << " " << (*m).start_index << " " << (*m).slope_index << "\n";
    }
    slope_motos = input_motos; // copy

    // iterate over motos
    int highest_start_treated = 0;
    int i = 0;
    while(highest_start_treated != n-1) {

        motorcycle slowest_slope_moto = slope_motos[i];

        if (slowest_slope_moto.start_index >= highest_start_treated) {
            highest_start_treated = slowest_slope_moto.start_index;
            slope_motos[i].active = true;
        }

        // cout << "debug " << i << " " << highest_start_treated << " " << slowest_slope_moto.input_index << " " <<
        //     slowest_slope_moto.start_index << " " << slowest_slope_moto.slope_index << "\n"; 

        i++;
    }

    // sort by input_index to satisfy output requirements
    std::sort(slope_motos.begin(), slope_motos.end(), sort_input);

    // printinput_motosikers
    for (int i=0; i<n; i++) {
        if (slope_motos[i].active) cout << slope_motos[i].input_index << " ";
    }
    cout << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while (t--) ride_forever();
}
