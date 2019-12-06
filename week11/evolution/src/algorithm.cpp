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

Final approach
Same as third, but instead of storing branches as vector<vector<string>>, store them as vector<vector<int>> where the int is the index of the specie.
Indeed, copying the common trunc in visit() was too slow.
Added index member in specie and vector index_to_string. And works !
Would be nice to check if the reason for the assertion error in judge was indeed a memory problem caused by copying large vector of string.

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

https://stackoverflow.com/questions/9132502/how-are-c-strings-stored
https://stackoverflow.com/questions/3557591/stdstring-and-its-automatic-memory-resizing
*/

struct specie {
    int index;
    static int branch_counter;
    int age;
    string direct_ancester; // to find root and start dfs
    vector<string> children; // for dfs
    int branch_number;
    int position_in_branch; // when several children and start new branch, to be able to keep common trunc
    bool visited; // for dfs
};
int specie::branch_counter = 0;

void visit(specie& current_specie, unordered_map<string, specie>& species, vector<vector<int>>& branches) {

    for (auto it = current_specie.children.begin(); it != current_specie.children.end(); ++it) {

        specie* child = &species[*it]; // access address to assign pointer

        if (!(*child).visited) {

            // determine branch number and update branches array
            if (it == current_specie.children.begin()) { // first child : same branch number
                (*child).branch_number = current_specie.branch_number;
                branches[(*child).branch_number].push_back((*child).index);
            }

            else { // create a new branch but keep trunc from previoud branch
                specie::branch_counter ++;
                (*child).branch_number = specie::branch_counter;
                branches[(*child).branch_number] = vector<int>(branches[current_specie.branch_number].begin(), 
                  branches[current_specie.branch_number].begin() + current_specie.position_in_branch + 1); // only keep trunc !! without + 1, we woulnt get the luca
                branches[(*child).branch_number].push_back((*child).index);
            }

            // recursive call
            (*child).visited = true;
            (*child).position_in_branch = current_specie.position_in_branch + 1;
            visit(*child, species, branches);
        }
    }
}

string bin_search(string current_specie, int query_age, unordered_map<string, specie>& species, vector<vector<int>>& branches, vector<string>& index_to_string) {

    // find corresponding branch
    vector<int> elected_branch = branches[species[current_specie].branch_number]; // vector of string of the species that current_specie belongs to

    // finally binary search
    int left = 0; // the root or luca
    int right = species[current_specie].position_in_branch; // position of current_specie on its branch. We dont start at the leaf, would be waste!
    string best_specie = index_to_string[elected_branch[right]]; // remember that branches stores indices of species, not their string
    int best_age = species[best_specie].age;

    while (left < right) {

        int middle = (left + right) / 2;
        string specie_middle = index_to_string[elected_branch[middle]];
        int middle_age = species[specie_middle].age;

        // check if middle age is acceptable
        if (middle_age <= query_age) { // middle age is acceptable
            right = middle;
            best_age = middle_age;
            best_specie = index_to_string[elected_branch[middle]];
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

    unordered_map<string, specie> species; // map from int and not string for memory concerns
    vector<string> index_to_string(n);

    for (int i = 0; i < n; i ++) {
        string name;
        int age;
        cin >> name >> age;
        species[name] = {i, age, "", vector<string>(), 0, 0, false};
        index_to_string[i] = name;
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
    int n_leaves = 0;

    for (auto it = species.begin(); it != species.end(); ++it) {

        if ((*it).second.direct_ancester.empty()) {
            root = &(it->second); // assign address to the pointer
        } 
        else if ((*it).second.children.size() == 0) {
            n_leaves ++;
        }
    }

    // create branch vector with n_leaves sub_vectors
    vector<vector<int>> branches(n_leaves, vector<int>({(*root).index})); // int and string so that doesnt get too big

    // run dfs from root and record branches
    (*root).visited = true; // dereference pointer to specie to access members of struct
    (*root).branch_number = 0; // was default but good to reassert it

    // start DFS to compute and store the branches (same number of branches as they are leaves)
    visit(*root, species, branches); // insurance that 1 tree with 1 luca

    // for each request, identify branch where the node lies. Then apply binary search on it
    for (int i = 0; i < q; i ++) {

        string current_specie;
        int query_age;
        cin >> current_specie >> query_age;

        string best_specie = bin_search(current_specie, query_age, species, branches, index_to_string);

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