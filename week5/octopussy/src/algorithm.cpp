#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

/*
First idea (no need to read!!!): keep track of available balls and a timer.
For each possible set of available balls, save a boolean to indicate if there is a way to deactivate set.
Seems exponential though. What is the number of possible sets of accessible balls???

Solution implemented here: use greedy approach: propagate time priorities dependencies in tree
Each node has as attribute the min of timers of all nodes that depend on it

Question for sweet Louis:
The chosen ball is deactivated and removed from the priority queue.
However, it seems these 2 operations were not taken into account.
Indeed, the same ball is being chosen several time and shows false boolean for its deactivation fild -_-

problem was in line 72, was deactivating ball from the heap but not from the all_balls vector.
Need to deactivate from the vector! (to check if brother deactivated)
Explanation: queue::push et vector::push_back pushent des copies et pas des references.

*/

using namespace std;

struct ball {

    int time;
    int min_dependency;
    bool deactivated;
    int position;

    // for priority queue
    bool operator<(const ball& other_ball) const {
        return min_dependency >= other_ball.min_dependency;
    }
};



void save_world() {

    int n, time;
    cin >> n;
    int rebours = 0;

    // put all balls in a vector and compute dependencies
    vector<ball> all_balls(n);
    cin >> time;
    all_balls[0] = ball({time, time, false, 0});
    for (int i=1; i<n; i++) {
        cin >> time;
        all_balls[i] = ball({time, time, false, i});
        all_balls[i].min_dependency = min(all_balls[i].time, all_balls[(i-1)/2].min_dependency);
    }

    // priority queue with accessible balls on the ground
    std::priority_queue<ball> accessible_balls;
    for (int i=(n-1)/2; i<n; i++) {
        accessible_balls.push(all_balls[i]);
    }

    // now greedy possible, based on min dependencies
    ball chosen_ball, brother;

    while(!accessible_balls.empty()) {

        chosen_ball = accessible_balls.top();
        // remove deactivated ball from accessible balls
        accessible_balls.pop();
        all_balls[chosen_ball.position].deactivated = true; // chosen_ball.deactivated = True WILL NOT WORK
        //cout << "chosen " << chosen_ball.position << " " << chosen_ball.min_dependency << " " << chosen_ball.deactivated << " " << rebours <<  "\n";

        if (rebours >= chosen_ball.min_dependency) {
            cout << "no\n";
            return;
        }

        if (chosen_ball.position == 0) break; // we just deactivated the parent and saved the world :-)

        // find brother of chosen ball
        if (chosen_ball.position%2==0) brother = all_balls[chosen_ball.position - 1];
        else brother = all_balls[chosen_ball.position + 1];

        // push brother parent to accessible balls if brother deactivated
        if (brother.deactivated) {// else push parent
            accessible_balls.push(all_balls[(chosen_ball.position-1)/2]);
            //cout << "parent pushed " << brother.position << " " << brother.deactivated << "\n";
        } 

        rebours ++;
    }

    cout << "yes\n";

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) save_world();
}
