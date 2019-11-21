#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>

// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>

// typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;
// typedef EK::Point_2 P;
// typedef EK::Line_2 L;
// typedef EK::Ray_2 R;
// typedef EK::Direction_2 D;

int INF = std::numeric_limits<int>::max();
using namespace std;
using namespace boost::multiprecision;

struct motorcycle {
    int128_t y0;
    int128_t x1;
    int128_t y1;
    int start_index;
    int slope_index;
    int input_index;
    bool active;
};

bool sort_start(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    return m1.y0 < m2.y0;
}

bool sort_slope_abs(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    if (abs(m1.y1 - m1.y0)*m2.x1 != abs(m2.y1 - m2.y0)*m1.x1) return abs(m1.y1 - m1.y0)*m2.x1 < abs(m2.y1 - m2.y0)*m1.x1;
    else if (m1.y1 >= m1.y0 && m2.y1 >= m2.y0) return m1.y0 < m2.y0;
    else if (m1.y1 < m1.y0 && m2.y1 < m2.y0) return m2.y0 < m1.y0;
    else return m1.y0 < m2.y0;
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
        input_motos.push_back({y0, x1, y1, i, i, i, false});
    }

    // sort starting points
    std::sort(input_motos.begin(), input_motos.end(), sort_start);
    for (int i=0; i<n; i++) {
        motorcycle* m = &input_motos[i]; // if no pointer, will make copy -> non persistent change
        (*m).start_index = i; 
        //cout << "sort start " << " " << " " << (*m).input_index << " " << (*m).start_index << "\n";
    }
    
    // sort slopes IN ABSOLUTE VALUE
    std::sort(input_motos.begin(), input_motos.end(), sort_slope_abs);
    for (int i=0; i<n; i++) {
        motorcycle* m = &input_motos[i]; // if no pointer, will make copy -> non persistent change
        (*m).slope_index = i;
        //cout << "sort slope " << " " << (*m).slope << " " << (*m).input_index << " " << (*m).start_index << " " << (*m).slope_index << "\n";
    }

    for (auto it = input_motos.begin(); it != input_motos.end(); ++it) {
        cout << (*it).input_index << " " << (*it).start_index << " " << (*it).slope_index << "\n";
    }


    // iterate over motos
    int highest_start_treated = 0;
    int lowest_start_treated = n-1;
    int i = 0;
    while(highest_start_treated != n-1 || lowest_start_treated != 0) {

        motorcycle m = input_motos[i];

        if (m.y1 >= m.y0 && m.start_index >= highest_start_treated) input_motos[i].active = true;
        else if (m.y1 <= m.y0 && m.start_index <= lowest_start_treated) input_motos[i].active = true;
        if (m.start_index >= highest_start_treated) {highest_start_treated = m.start_index;}
        if (m.start_index <= lowest_start_treated) {lowest_start_treated = m.start_index;}
        
        i++;
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
