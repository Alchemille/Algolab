#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <utility>
#include <queue> // needed for priority queue

/*
https://stackoverflow.com/questions/15601973/stl-priority-queue-of-c-with-struct
https://www.geeksforgeeks.org/priority_queuetop-c-stl/
https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back
https://stackoverflow.com/questions/8067338/vector-of-structs-initialization
https://stackoverflow.com/questions/54585/when-should-you-use-a-class-vs-a-struct-in-c
https://www.geeksforgeeks.org/implement-min-heap-using-stl/


problems
- did not work to update end_boat in push_to_fit. Must always top() and pop() and eventually repush with new end_boat ->  c++ priority queue does not support increase key.
    Need to explicitly rebalance or pop then repush (logn)
- redeclaration of Boat messed up if put type -> means the new definition is valid only in inner block, then former definition used again
- why is inner while loop bad for complexity -> there is infinite loop with test2, boats2
- when is while loop condition checked -> at curly bracket
*/

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
            cout << last_len_boat << " " << len_boat << " " << end_boat << "\n";
            if (ancrage >= last_len_boat) {
                // push boat
                //cout << "1\n";
                //end_boat = last_len_boat + len_boat;
                return 1;
            }
            else return 2;
        }
        else return 0;
    }
};

void boats() {

    // read inputs
    int n, l, p, result = 0;
    cin >> n;

    // define and initialize boats as a 
    std::priority_queue<Boat> boats;
    for (int i=0; i<n; i++){
        cin >> l >> p;
        boats.push({l, p, p});
    }
    
    Boat::last_len_boat = boats.top().ancrage - boats.top().len_boat - 1;
    while (!boats.empty()) {

        // extract candidate boat with smallest finishing time possible
        Boat candidate = boats.top();
        int status_heap = candidate.push_to_fit();

        while (status_heap != 0) { 
            // 0 -> heap was not updated, top is definitly best candidate and fits
            // 1 -> candidate fits but had to be pushed. Another candidate might be better
            // 2 -> candidate did to fit. To be removed.
            // accepted candidate should have smallest finishing point, not ancrage, be accessible from ancrage and not overlap
            //cout << status_heap << " " << Boat::last_len_boat << " " << candidate.ancrage << " " << candidate.len_boat  << " " << candidate.end_boat << "\n";            
            boats.pop();
            if (status_heap == 1) boats.push({candidate.len_boat, candidate.ancrage, Boat::last_len_boat + candidate.len_boat});

            // extract next candidate
            candidate = boats.top(); // candidate fits and was moved
            //cout << status_heap << " " << Boat::last_len_boat << " " << candidate.ancrage << " " << candidate.len_boat << " " << candidate.end_boat << "\n";
            status_heap = candidate.push_to_fit();
        }

        // account for accepted candidate and update variables
        result ++;
        boats.pop();
        Boat::last_len_boat = candidate.end_boat;
        cout << "chosen " << status_heap << " " << Boat::last_len_boat << " " << candidate.ancrage << " " << candidate.len_boat  << " " << candidate.end_boat << "\n";

    }

    cout << result << "\n";
}

int Boat::last_len_boat;
int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) boats();
}
