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
https://www.hackerrank.com/challenges/crush/problem

mysort without custom modulo function was wrong. it - origin could be negative. Need to force it to be positive.

!!! To determine segement with minimum overlapping jedi !!!
If want to store compteur pour chaque balise and then sum the balises,
not use unordered map (iterator to sum needs to take segment in right order)
Also not use vector of size m, would be too big.
Sol : map or vector of pairs that is sorted by key.
Directly computing sum does not seem possible. 
For balise fermante, careful to decrement the next segment and use modulo !!!


!!! Careful when enonce with intervals/sets !!!
here, [a, b] includes segment a to segement b INCLUDED


compute_n_jedi: Solution Louis does not require to resort for every overlapping jedi.
*/

using namespace std;

bool compare_end(pair<int, int>& i1, pair<int, int>& i2) {
    if (i1.second == i2.second) return i1.first < i2.first;
    else return i1.second < i2.second;
};

// Pour rester dans l'intevalle [0, m)
long mod(long a, long b) { return (a + b) % b; }

void my_sort(vector<pair<int, int>> &jedi, long origin, long m) {
    for (auto &it : jedi) { // VERY COOL
        it.first = mod(it.first - origin, m); // ALSO COOLER
        it.second = mod(it.second - origin, m);
    }

    sort(jedi.begin(), jedi.end(), compare_end);
}

long compute_n_jedi(vector<pair<int, int>> jedi, pair<int, int> starting_int, long m) { // pass by copy since resort
    // apply interval scheduling with starting_jedi as origin

    // sort according to finish times in referential of starting_m
    long origin = starting_int.first;
    my_sort(jedi, origin, m);
    long current_jedi = 1; // account for the segment we start with
    long current_finish = mod(starting_int.second - origin, m); // recompute the end of the first segment jedi
    // apply interval scheduling

    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {
        if ((*it).first > current_finish && (*it).second >= (*it).first ) { // >= : there could be only 1 segment
            // add this jedi to army
            current_jedi ++;
            current_finish = (*it).second;
        }
    }
    return current_jedi;
}


long compute_n_jedi_no_overlap(vector<pair<int, int>>& jedi, long m) {

    long current_jedi = 0, current_finish = 0;

    for (vector<pair<int, int>>::iterator it=jedi.begin(); it!=jedi.end(); ++it) {

        // importance of >= below: case with only 1 segemnt, so first = second
        if ((*it).second >= (*it).first && (*it).first != 0 && (*it).second != 0) { // not overlapping jedi int
            if ((*it).first > current_finish) {
                // add this jedi to army
                current_jedi ++;
                current_finish = (*it).second;
            }
        }
    }
    return current_jedi;
}


long find_interval(long n, long m, vector<pair<int, int>>& jedi) {
    
    map<int, int> counter;

    for (auto &it : jedi) {
        counter[mod(it.first, m)] ++;
        counter[mod(it.second + 1, m)] --;
    }

	long elected_interval = -1;
	long min_k = n;
	long k = 0;
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

    long u, v, n, m, chosen_int;
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
    long n_army = 0, n_army_cand, n_cand = 0;
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

