#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

/*
Important:
Avoid to compute slopw and divide.
Instead, multiply to compare slopes.
Use multipresision since int not enough

Very tricky: sorting sort_slope_abs. Careful to have a consistent ordering.
Last condition (same abs rate and opposite direction):
Had said that the one starting below is treated before.
But shoul be the one going up.
*/

struct motorcycle {
    int128_t y0;
    int128_t x1;
    int128_t y1;
    int slope_index;
    int input_index;
    bool active;
};

bool sort_slope_abs(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    if (abs(m1.y1 - m1.y0)*m2.x1 != abs(m2.y1 - m2.y0)*m1.x1) return abs(m1.y1 - m1.y0)*m2.x1 < abs(m2.y1 - m2.y0)*m1.x1;
    else if (m1.y1 >= m1.y0 && m2.y1 >= m2.y0) return m1.y0 < m2.y0; // parallel upward
    else if (m1.y1 < m1.y0 && m2.y1 < m2.y0) return m2.y0 < m1.y0; // parallel downward
    else return m1.y0 < m1.y1; // case where intersect at the same time. Priority to right.
}

bool sort_input(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    return m1.input_index < m2.input_index;
}

void ride_forever() {

    long n; 
    cin >> n;
    vector<motorcycle> input_motos;

    // read direction of each biker
    int128_t y0, x1, y1;

    for (int i=0; i<n; i++) {
        cin >> y0 >> x1 >> y1;
        input_motos.push_back({y0, x1, y1, i, i, false});
    }
    
    // sort slopes IN ABSOLUTE VALUE
    std::sort(input_motos.begin(), input_motos.end(), sort_slope_abs);
    for (int i=0; i<n; i++) {
        motorcycle* m = &input_motos[i]; // if no pointer, will make copy -> non persistent change
        (*m).slope_index = i;
    }

    // iterate over motos
    int128_t highest_start_treated = input_motos[0].y0;
    int128_t lowest_start_treated = input_motos[0].y0;
    
    for (int i=0; i<n; i++) {

        motorcycle m = input_motos[i];

        if (m.y1 >= m.y0 && m.y0 >= highest_start_treated) input_motos[i].active = true;
        else if (m.y1 <= m.y0 && m.y0 <= lowest_start_treated) input_motos[i].active = true;
        if (m.y0 >= highest_start_treated) {highest_start_treated = m.y0;}
        if (m.y0 <= lowest_start_treated) {lowest_start_treated = m.y0;}
    }

    // sort by input_index to satisfy output requirements
    std::sort(input_motos.begin(), input_motos.end(), sort_input);

    for (int i=0; i<n; i++) {
        if (input_motos[i].active) cout << input_motos[i].input_index << " ";
    }
    cout << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    while (t--) ride_forever();
}
