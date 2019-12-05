#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

struct specie {
    static int branch_counter;
    int age;
    string direct_ancester; // to find root and start dfs
    vector<string> children; // for dfs
    int branch_number;
    int position_in_branch; // when several children and start new branch, to be able to keep common trunc
    bool visited; // for dfs
};
int specie::branch_counter = 0;

void visit(specie& current_specie, unordered_map<string, specie>& species, vector<vector<string>>& branches) {

    for (auto it = current_specie.children.begin(); it != current_specie.children.end(); ++it) {

        specie* child = &species[*it]; // access address to assign pointer

        if (!(*child).visited) {

            // determine branch number and update branches array
            if (it == current_specie.children.begin()) { // first child : same branch number
                (*child).branch_number = current_specie.branch_number;
                branches[(*child).branch_number].push_back(*it);
            }

            else { // create a new branch but keep trunc from previoud branch
                specie::branch_counter ++;
                (*child).branch_number = specie::branch_counter;
                branches[(*child).branch_number] = vector<string>(branches[current_specie.branch_number].begin(), 
                  branches[current_specie.branch_number].begin() + current_specie.position_in_branch + 1); // only keep trunc !! without + 1, we woulnt get the luca
                branches[(*child).branch_number].push_back(*it);
            }

            // recursive call
            (*child).visited = true;
            (*child).position_in_branch = current_specie.position_in_branch + 1;
            visit(*child, species, branches);
        }
    }
}

string bin_search(string current_specie, int query_age, unordered_map<string, specie>& species, vector<vector<string>>& branches) {

    // find corresponding branch
    vector<string> elected_branch = branches[species[current_specie].branch_number];

    // finally binary search
    int left = 0; // the root or luca
    int right = species[current_specie].position_in_branch; // position of current_specie on its branch
    int best_age = species[elected_branch[right]].age;
    string best_specie = elected_branch[right];

    while (left < right) {

        int middle = (left + right) / 2;
        int middle_age = species[elected_branch[middle]].age;

        // check if middle age is acceptable
        if (middle_age <= query_age) { // middle age is acceptable
            right = middle;
            best_age = middle_age;
            best_specie = elected_branch[middle];
        }
        else {
            left = middle + 1;
        }
    }
    return best_specie;
}

void testcase() {

    int n, q;
    cin >> n >> q; 
    specie::branch_counter = 0; // important to restart at the beginning of each test!

    unordered_map<string, specie> species;

    for (int i = 0; i < n; i ++) {
        string name;
        int age;
        cin >> name >> age;
        species[name] = {age, "", vector<string>(), 0, 0, false};
    }

    for (int i = 0; i < n-1; i ++) {
        string child, parent;
        cin >> child >> parent;
        species[child].direct_ancester = parent;
        species[parent].children.push_back(child);
    }

    // Build branches from root to leaf with DFS

    // fist, find root. !! super careful: want specie for root to be a pointer, not a copie !!
    specie* root;
    string root_str;
    int n_leaves = 0;

    for (auto it = species.begin(); it != species.end(); ++it) {

        if ((*it).second.direct_ancester.empty()) {
            root = &(it->second); // assign address to the pointer
            root_str = it->first;
        } 
        else if ((*it).second.children.size() == 0) {
            n_leaves ++;
        }
    }

    // create branch vector with n_leaves sub_vectors
    vector<vector<string>> branches(n_leaves, vector<string>({root_str}));

    // run dfs from root and record branches
    (*root).visited = true; // dereference pointer to specie to access members of struct
    (*root).branch_number = 0; // was default but good to reassert it

    visit(*root, species, branches); // insurance that 1 tree with 1 luca

    // for each request, identify branch where the node lies. Then apply binary search on it
    for (int i = 0; i < q; i ++) {

        string current_specie;
        int query_age;
        cin >> current_specie >> query_age;

        string best_specie = bin_search(current_specie, query_age, species, branches);

        // !! in other exercises, didnt care if additional space at endofline. But matters here !!
        cout << best_specie;
        if (i < q-1) { 
            cout << " ";
        }
    }
    cout << "\n";
}   

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
