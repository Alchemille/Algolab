#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue> // needed for priority queue

using namespace std;

struct Boat {

    int len_boat;
    int ancrage;
    int end_boat;
    static int last_len_boat;

    bool operator<(const Boat& other_boat) const { // !! default priority queue is a max heap
        if (end_boat == other_boat.end_boat) { // take the one with smallest ancrage
            return (ancrage > other_boat.ancrage);
        }
        else return end_boat > other_boat.end_boat;
    }

    int push_to_fit() {
        // detect if conflict
        if (end_boat - len_boat < last_len_boat) {
            if (ancrage >= last_len_boat) return 1; 
            else return 2;
        }
        else return 0;
    }
};

void boats() {

    // read inputs
    int n, l, p, result = 0;
    cin >> n;
    int count = 0;

    // define and initialize boats as a 
    std::priority_queue<Boat> boats;
    for (int i=0; i<n; i++){
        cin >> l >> p;
        boats.push({l, p, p});
    }
    
    Boat::last_len_boat = boats.top().ancrage - boats.top().len_boat - 1;
    while (!boats.empty()) {

        count ++;

        // extract candidate boat with smallest finishing time possible
        Boat candidate = boats.top();
        int status_heap = candidate.push_to_fit();
        boats.pop();

        if (status_heap == 1) boats.push({candidate.len_boat, candidate.ancrage, Boat::last_len_boat + candidate.len_boat});
        else if (status_heap == 0) {
            result ++;
            Boat::last_len_boat = candidate.end_boat;
        }

    }

    cout << result << " " << count << "\n";
}

int Boat::last_len_boat;
int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) boats();
}
