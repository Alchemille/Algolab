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

bool sort_slope_abs(motorcycle const& m1, motorcycle const& m2) { // returns true if m1<m2
    if (abs(m1.slope) != abs(m2.slope)) return abs(m1.slope) < abs(m2.slope);
    // 3 cases where equal absolute slope
    else if (m1.slope >= 0 && m2.slope >= 0) return m1.start < m2.start;
    else if (m1.slope <= 0 && m2.slope <= 0) return m1.start > m2.start;
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
    
    // sort slopes IN ABSOLUTE VALUE
    std::sort(input_motos.begin(), input_motos.end(), sort_slope_abs);
    for (int i=0; i<n; i++) {
        motorcycle* m = &input_motos[i]; // if no pointer, will make copy -> non persistent change
        (*m).slope_index = i;
        //cout << "sort slope " << " " << (*m).slope << " " << (*m).input_index << " " << (*m).start_index << " " << (*m).slope_index << "\n";
    }
    slope_motos = input_motos; // copy. actueally, could have kept input_motos

    // iterate over motos
    int highest_start_treated = 0;
    int lowest_start_treated = n-1;
    int i = 0;
    while(highest_start_treated != n-1 || lowest_start_treated != 0) {

        if (slope_motos[i].slope >= 0. && slope_motos[i].start_index >= highest_start_treated) slope_motos[i].active = true;
        if (slope_motos[i].slope <= 0. && slope_motos[i].start_index <= lowest_start_treated) slope_motos[i].active = true;
        if (slope_motos[i].start_index >= highest_start_treated) {highest_start_treated = slope_motos[i].start_index;}
        if (slope_motos[i].start_index <= lowest_start_treated) {lowest_start_treated = slope_motos[i].start_index;}
        
        i++;
    }

    // sort by input_index to satisfy output requirements
    std::sort(slope_motos.begin(), slope_motos.end(), sort_input);

    for (int i=0; i<n; i++) {
        if (slope_motos[i].active) cout << slope_motos[i].input_index << " ";
    }
    cout << "\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(0);
    
    int t;
    cin >> t;
    while (t--) ride_forever();
}
