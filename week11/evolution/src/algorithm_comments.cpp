#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

/*
First approach:
use a map to direct ancester. In O(n*q). Hence 1.10^9 operations and 3s. Too slow.

Second approach:
Maybe shoulnt store all branches (redundancy)
Identify all leaves.
Then used direct_ancester to build all branches from leaf to root. Each node which branch it is part of.
For each query, identify branch and apply binary search on the branch
Complexity O(q*log(n) + n*n) too slow :-(

Third approach
Instead of storing branches from leaft to root and repeating common trunc,
store branches from root to leaf by using DFS/BFS. 
Once branches stored, apply binary search

careful to:
- spaces in output
- custom comparator parameter -> use lambda function 
https://stackoverflow.com/questions/4066576/passing-a-parameter-to-a-comparison-function
https://stackoverflow.com/questions/11556394/initializing-strings-as-null-vs-empty-string
manipulation of collection of structure:
careful when changing a member: for persistence, use pointers / reference
branch counter is static
https://stackoverflow.com/questions/1120744/is-it-possible-to-add-a-static-variable-to-a-c-struct/45297440
https://stackoverflow.com/questions/50549611/slicing-a-vector-in-c
https://linuxize.com/post/diff-command-in-linux/
./build/release/algo < sample.in | diff -b - test4.out
https://www.geeksforgeeks.org/initialize-a-vector-in-cpp-different-ways/
*/

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

void print_tree(unordered_map<string, specie>& species) {
    cout << "branch counter " << specie::branch_counter << "\n";
    for (auto it = species.begin(); it != species.end(); ++it) {
        specie current_specie = (*it).second;
        cout << (*it).first << " ";
        cout << "branch number " << current_specie.branch_number << 
                " ancester " << current_specie.direct_ancester << 
                " age " << current_specie.age <<
                " visited " << current_specie.visited << "\n";
    }
}

void visit(specie& current_specie, unordered_map<string, specie>& species, vector<vector<string>>& branches) {

    for (auto it = current_specie.children.begin(); it != current_specie.children.end(); ++it) {

        specie* child = &species[*it]; // access address to assign pointer

        if (!(*child).visited) {

            // determine branch number and update branches array
            if (it == current_specie.children.begin()) { // first child : same branch number
                (*child).branch_number = current_specie.branch_number;
                // cerr << branches.size() << " " << (*child).branch_number << "\n";
                // if ((*child).branch_number >= branches.size()) cerr << "segfault\n";

                branches[(*child).branch_number].push_back(*it);
            }

            else { // create a new branch but keep trunc from previoud branch
                specie::branch_counter ++;
                (*child).branch_number = specie::branch_counter;
                // cerr << branches.size() << " " << (*child).branch_number << "\n";
                // if ((*child).branch_number >= branches.size()) cerr << "segfault\n";

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

    // // DEBUG: print branches
    // for (int i = 0; i < branches.size(); i ++) {
    //     cout << "new branch ";
    //     for (int j = 0; j < branches[i].size(); j ++) {
    //         cout << branches[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

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




// BELOW: Thought that result should be printer in order of input index

// // output result species ordered with input. Use lambda function since need to use map index_input inside the custom comparator
// auto my_comp = [&] (string s1, string s2) -> bool {
//     return index_input[s1] < index_input[s2];
// };

// sort(result.begin(), result.end(), my_comp);

// for (auto it = result.begin(); it != result.end(); ++ it) {
//     cout << *it << " ";
// }