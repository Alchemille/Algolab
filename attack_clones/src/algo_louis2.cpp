#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>


using namespace std;

// Pour rester dans l'intevalle [0, m)
int mod(int a, int b) { return (a % b + b) % b; }

bool compare_end(const pair<int, int> &i1, const pair<int, int> &i2) {
    if (i1.second == i2.second) return i1.first < i2.first;
    else return i1.second < i2.second;
};

void my_sort(vector<pair<int, int>> &jedi, int origin, int m) {
    for (auto &it : jedi) { // VERY COOL
        it.first = mod(it.first - origin, m); // ALSO COOLER
        it.second = mod(it.second - origin, m);
    }

    sort(jedi.begin(), jedi.end(), compare_end);
}

int compute_n_jedi(vector<pair<int, int>> jedi, int selected_jedi, int m) {
    // sort according to finish times in referential of start
    // On n'a besoin de trier qu'une seule fois!
//    int origin = jedi[selected_jedi].first;
//    my_sort(jedi, origin, m);
    int num_jedi = 1;
    int current_finish = jedi[selected_jedi].second;
    // apply interval scheduling

    // On ne regarde que les jedis qui arrvent après le jedi sélectionné
    for (unsigned long i = selected_jedi + 1; i < jedi.size(); i++) {
        // Condition 2: soit le jedi sélectionné commence à 0, soit on s'arrête quand on atteint le début du jedi sélectionné
        if (jedi[i].first > current_finish &&
            (jedi[selected_jedi].first == 0 || jedi[i].second < jedi[selected_jedi].first)) {
            // add this jedi to army
            num_jedi++;
            current_finish = jedi[i].second;
        }
    }
    return num_jedi;
}

int compute_n_jedi_no_overlap(const vector<pair<int, int>> &jedi, int m) {

    int num_jedi = 0, current_finish = 0;
    for (auto &it : jedi) { // PK NE MARCHE PAS SANS LE CONST
        if (it.second < it.first || it.first == 0 ||
            it.second == 0) { // on n'aime pas les jedi qui protègent 0 (inclus, à gauche, à droite)
            continue;
        }
        if (it.first > current_finish) {
            // add this jedi to army
            num_jedi++;
            current_finish = it.second;
        }
    } 
    return num_jedi;
}

int find_interval(int n, int m, const vector<pair<int, int>> &jedi) { // PK UTILISER CONST
    map<int, int> counter;

    for (auto &it : jedi) {
        counter[mod(it.first, m)]++; // POURQUOI PRENDRE LE MODULO
        counter[mod(it.second + 1, m)]--; // PK +1 
    }

    int argmin = -1;
    int min_k = n;
    int sum = 0;
    for (auto &it : counter) {
        sum += it.second;
        if (sum < min_k) {
            argmin = it.first;
            min_k = sum;
        }
    }
    //cerr << "best " << argmin << " " << min_k << endl;
    return argmin;
}

void attack() {
    int n, m;
    cin >> n >> m;

    vector<std::pair<int, int>> jedi(n, {0, 0});
    for (int i = 0; i < n; i++) {
        int u, v;
        cin >> u >> v;
        jedi[i].first = u - 1; // réindexation dans [0, m)
        jedi[i].second = v - 1;
    }

    // find first segment with <= 10 jedi. ie determine chosen_int with sliding window
    int chosen_int = find_interval(n, m, jedi);

    // sort intervals according to chosen_start
    my_sort(jedi, chosen_int, m);

    // apply interval scheduling over all possible chosen jedi
    int n_army = 0;
    int n_cand = 0;
    int n_army_cand;
    for (int i = 0; i < n; i++) {
//        cerr << (*it).first << " " << (*it).second << endl;
        auto elem = jedi[i];
        if (elem.second < elem.first || elem.first == 0) { // le jedi overlap 0
            n_cand++;
            n_army_cand = compute_n_jedi(jedi, i, m);
            n_army = max(n_army, n_army_cand);
        }
    }
    n_army_cand = compute_n_jedi_no_overlap(jedi, m); 
    //cerr << chosen_int << " " << n_army << " " << n_army_cand << " " << n_cand << "\n";
    n_army = max(n_army, n_army_cand);

    cout << n_army << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) attack();
}