#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>
#include <unordered_map>

/*
Important to index from 0 if plan to use modulo
iterator: can use auto& it : vector, a bit like in java

https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
https://stackoverflow.com/questions/16772842/what-is-the-difference-between-cout-cerr-clog-of-iostream-header-in-c-when

mysort without custom modulo function was wrong. it - origin could be negative. Need to force it to be positive.
*/

using namespace std;

bool compare_end(pair<int, int>& i1, pair<int, int>& i2) {
    if (i1.second == i2.second) return i1.first < i2.first;
    else return i1.second < i2.second;
};

// Pour rester dans l'intevalle [0, m)
int mod(int a, int b) { return (a % b + b) % b; }

void my_sort(vector<pair<int, int>> &jedi, int origin, int m) {
    for (auto &it : jedi) { // VERY COOL
        it.first = mod(it.first - origin, m); // ALSO COOLER
        it.second = mod(it.second - origin, m);
    }

    sort(jedi.begin(), jedi.end(), compare_end);
}

int compute_n_jedi(vector<pair<int, int>> jedi, pair<int, int> starting_int, int m) { // pass by copy since resort
    // apply interval scheduling with starting_jedi as origin

    // sort according to finish times in referential of starting_m
    int origin = starting_int.first;
    my_sort(jedi, origin, m);
    int current_jedi = 1;
    int current_finish = starting_int.second;
    // apply interval scheduling

    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).first > current_finish && (*it).second > (*it).first) {
            // add this jedi to army
            current_jedi ++;
            current_finish = (*it).second;
        }
    }
    return current_jedi;
}


int compute_n_jedi_no_overlap(vector<pair<int, int>>& jedi, int m) {

    int current_jedi = 0, current_finish = 0;
    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).second > (*it).first && (*it).first != 0) { // not overlapping jedi int
            if ((*it).first > current_finish && (*it).second > (*it).first) {
                // add this jedi to army
                current_jedi ++;
                current_finish = (*it).second;
            }
        }
    }
    return current_jedi;
}


int find_interval(int n, int m, vector<pair<int, int>>& jedi) {
    
    unordered_map<int, int> counter;

    for (auto &it : jedi) {
        counter[it.first] ++;
        counter[mod(it.second + 1, m)] --;
    }

	int elected_interval = -1;
	int min_k = n;
	int k = 0;
	for(auto it = counter.begin(); it != counter.end(); ++it) {
		k += it->second;
		if(k < min_k) {
			elected_interval = it->first;
			min_k = k;
		}
	}
    return elected_interval;
}

void attack() {

    int u, v, n, m, chosen_int;
    cin >> n >> m;

    vector<std::pair<int, int>> jedi(n, {0, 0});
    for (int i=0; i<n; i++) {
        cin >> u >> v;
        jedi[i].first = u - 1;
        jedi[i].second = v - 1;
    }

    // find first segment with <= 10 jedi.
    chosen_int = find_interval(n, m, jedi);

    // sort intervals according to chosen_start
    my_sort(jedi, chosen_int, m);

    // apply interval scheduling over all possible chosen jedi
    int n_army = 0, n_army_cand, n_cand = 0;
    int i = 0;
    for (vector<pair<int, int>>::iterator it = jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).second < (*it).first || (*it).first == 0) { // *it is a possible jedi for interval scheduling
            n_army_cand = compute_n_jedi(jedi, *it, m);
            n_army = max(n_army, n_army_cand);
            n_cand ++;
        } 
        i++;
    }
    // last case, not choose one of the overlapping jedis
    n_army_cand = compute_n_jedi_no_overlap(jedi, m); 
    cerr << chosen_int << " " << n_army << " " << n_army_cand << " " << n_cand << "\n";
    n_army = max(n_army, n_army_cand);

    cout << n_army << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) attack();
}

