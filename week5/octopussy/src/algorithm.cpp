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

Then thought about DP, but did not lead anywhere.

Solution implemented here: use greedy approach: propagate time priorities dependencies in tree,
from parent to its kids.
Each node has as attribute the min of timers of all nodes that depend on it

Trick copies when container of structure:
The chosen ball is deactivated and removed from the priority queue.
However, it seems these 2 operations were not taken into account.
Indeed, the same ball is being chosen several time and shows false boolean for its deactivation fild -_-
problem was in line 72, was deactivating ball from the heap but not from the all_balls vector.
Need to deactivate from the vector! (to check if brother deactivated)
Explanation: queue::push et vector::push_back pushent des copies et pas des references.

Implementations:
- with min heap: only push accessible balls. When 2 brothers deactivated, push parent
- with simple vector or queue, much more simple : push all in priority order but be careful to push kids before parents. 

Morale: queue sufficient si pas besoin de pop et repusher des elements, comme dans Boats par ex.

Equivalent smart solution, kinda DP:
https://github.com/AngelaZhouETH/AlgoLab/blob/master/octopussy.cpp
instead of hidden most urgent timer behind each node, compute time by which node must be visited so that nodes behind can be deactivated
Generally, when DP on tree, children are subproblems of parent
*/

using namespace std;

struct ball {

    int time;
    int min_dependency;
    int position;
};

void save_world() {

    int n, time;
    cin >> n;
    int rebours = 0;

    // put all balls in a vector and compute dependencies
    vector<ball> all_balls(n);
    cin >> time;
    all_balls[0] = ball({time, time, 0});
    for (int i=1; i<n; i++) {
        cin >> time;
        all_balls[i] = ball({time, time, i});
        all_balls[i].min_dependency = min(all_balls[i].time, all_balls[(i-1)/2].min_dependency);
    }

    // sort balls by increasing min_dependency and make sure children before their parents
    sort(all_balls.begin(), all_balls.end(), [] (const ball& b1, const ball& b2) -> bool {
       if (b1.min_dependency == b2.min_dependency) {
           return b1.position > b2.position;
       } 
       else return b1.min_dependency < b2.min_dependency;
    });

    std::vector<ball> balls_queue;
    for (ball balll : all_balls) {
        balls_queue.push_back(balll);
    }

    // now greedy possible, based on min dependencies
    for (ball chosen_ball : balls_queue) {

        if (rebours >= chosen_ball.min_dependency) {
            cout << "no\n";
            return;
        }

        if (chosen_ball.position == 0) break; // we just deactivated the parent and saved the world :-)

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
