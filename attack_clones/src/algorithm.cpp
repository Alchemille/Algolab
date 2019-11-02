#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>


using namespace std;

bool compare_end(pair<int, int>& i1, pair<int, int>& i2) {
    if (i1.second == i2.second) return i1.first < i2.first;
    else return i1.second < i2.second;
};

bool compare_start(pair<int, int>& i1, pair<int, int>& i2) {
    if (i1.first == i2.first) return i1.second < i2.second;
    else return i1.first < i2.first;
};

void my_sort(vector<pair<int, int>>& jedi, int origin, int m) {
    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {
        (*it).first = ((*it).first - origin) % m;
        (*it).second = ((*it).second - origin) % m;
    }

    sort(jedi.begin(), jedi.end(), compare_end);    
}

int compute_n_jedi(vector<pair<int, int>> jedi, pair<int, int> starting_int, int m) {
    // sort according to finish times in referential of start

    int origin = starting_int.first;
    my_sort(jedi, origin, m);
    int current_jedi = 1;
    int current_finish = (starting_int.second - starting_int.first) % m;
    // apply interval scheduling

    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).first > current_finish && (*it).second > current_finish && (*it).second > (*it).first) {
            // add this jedi to army
            current_jedi ++;
            current_finish = (*it).second;
        }
    }
    return current_jedi;
}

int compute_n_jedi_no_overlap(vector<pair<int, int>> jedi, int m) {

    int current_jedi = 0, current_finish = 0;
    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).second > (*it).first) { // not overlapping jedi int
            if ((*it).first > current_finish && (*it).second > current_finish && (*it).second > (*it).first) {
                // add this jedi to army
                current_jedi ++;
                current_finish = (*it).second;
            }
        }
    }
    return current_jedi;
}

// int find_interval(int n, int m, vector<pair<int, int>> jedi) {
//     // compute number of overlapping jedis for each interval in a sliding window fashion
//     // first create list of 2n sorted events (open and close)
//     vector<pair<int, bool>> sorted_events(2*n); // the bool indicates whether starting event
//     vector<pair<int, int>> overlapped_jedis_per_segment(m, {0, 2});
//     for(int i=0; i<n; i++) {
//         sorted_events[i].first = jedi[i].first;
//         sorted_events[i].second = 0; // is a starting event
//         sorted_events[i+n].first = jedi[i].second;
//         sorted_events[i+n].second = 1; // is an ending event
//     }
//     sort(sorted_events.begin(), sorted_events.end(), compare_start); // sort along date of event (refereential doesnt matter)

//     int outstanding_open = 0;
//     int j = 0;

// }

int find_interval(int n, vector<pair<int, int>> jedi) {
    
    map<int, int> counter;

    for (vector<pair<int, int>>::iterator it = jedi.begin(); it!=jedi.end(); ++it) {
        counter[(*it).first] ++;
        counter[(*it).second] --;
    }

	int o = -1;
	int min_k = n;
	int k = 0;
	for(auto it = counter.begin(); it != counter.end(); ++it) {
		k += it->second;
		if(k <= 10 && k < min_k) {
			o = it->first;
			min_k = k;
		}
	}
    return o;
}

void attack() {

    int u, v, n, m, chosen_int;
    cin >> n >> m;

    vector<std::pair<int, int>> jedi(n, {0, 0});
    for (int i=0; i<n; i++) {
        cin >> u >> v;
        jedi[i].first = u;
        jedi[i].second = v;
    }

    // find first segment with <= 10 jedi. ie determine chosen_int with sliding window
    chosen_int = find_interval(n, m, jedi);

    // sort intervals according to chosen_start
    my_sort(jedi, chosen_int, m);

    // apply interval scheduling over all possible chosen jedi
    int n_army = 0, n_army_cand;
    for (vector<pair<int, int>>::iterator it = jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).second < (*it).first) { // *it is a possible jedi for interval scheduling
            n_army_cand = compute_n_jedi(jedi, *it, m);
            n_army = max(n_army, n_army_cand);
        } 
    }
    // last case, not choose one of the overlapping jedis
    n_army_cand = compute_n_jedi_no_overlap(jedi, m); 
    n_army = max(n_army, n_army_cand);

    cout << n_army << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) attack();
}

