#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <set>
#include <bits/stdc++.h>
#include <unordered_map>
#include <boost/functional/hash.hpp>

/*
2e revue:
Could have clean sliding window with i and j (relative positions)
min heap useless, queue simpler
even simple queue is useless since all to dequeue is current i
begin by pushing j to get all elements.
Then do while(j < n_positions) explore new word
while (word at i redundant) push i

Idea of algo
Maintain both a map (word->position) of the latest position for each word, and a
min heap (position, word) where the heap is based on the position.
If discover new word, push it to map and queue. Updtate max position.
If word was already there, push it anyway to queue and replace postion in map. Update max_positon
    Then check that the top of the queue is useless, ie if its word is present later in the heap. 
    Can be checked in constant time by comparing positions stored in the map and in the queue.
    While the top is useless, pop it and update the min.
If all the words contained in map, check min and max to see if the lenght is better


- custom comparator for sorting vector
- custom comparator for sorting min priority queue, and other arguments in constructor
- map of pairs needs <boost/functional/hash.hpp> and other arguments in constructor

Trick / Traps
- comparator for min heap is read as for a max heap
- segfault could happen if size of vector not specified
*/

using namespace std;

bool compare_positions(pair<int, int> p1, pair<int, int> p2) {
    // used both to order words positions of documuments and min priority queue
    return p1.first < p2.first; // sort according to position in the document
}

class myComparator 
{ 
public: 
    int operator() (const pair<int, int> p1, const pair<int, int> p2) 
    { 
        return !compare_positions(p1, p2); // careful to invert boolean, since transform max heap in min heap 
    } 
};

void testcase() {

    // read input
    int n, n_positions = 0;
    cin >> n;
    vector<int> n_occurrences(n, 0);
    for (int i=0; i<n; i++) {
        cin >> n_occurrences[i];
        n_positions += n_occurrences[i];
    }

    vector<pair<int, int>> all_positions(n_positions); // a pair is {position of word, word}
    int k = 0;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n_occurrences[i]; j++) {
            int pos;
            cin >> pos;
            all_positions[k] = {pos, i};
            k ++;
        }
    }

    // sort array with all the positions
    sort(all_positions.begin(), all_positions.end(), compare_positions);

    // create map to contain all words currently included
    unordered_map<int, int> words_map; // word -> position. Latest positions of words to be included 
    priority_queue<pair<int, int>, vector<pair<int, int>>, myComparator> words_queue;

    int abs_position =all_positions[0].first;
    int word = all_positions[0].second;

    words_map[word] = abs_position;
    words_queue.push(all_positions[0]); // careful, [abs_position, word] unlike map word -> abs_position

    int min_position = abs_position;
    int max_position = abs_position;

    int relative_position = 1;
    int best_length = std::numeric_limits<int>::max();
    
    while(relative_position < n_positions-1) { // first word already treated before

        abs_position = all_positions[relative_position].first;
        word = all_positions[relative_position].second;
        words_queue.push({abs_position, word});
        words_map[word] = abs_position;
        max_position = abs_position;

        // if word already in map, replace it and update min and max positions.
        // max easy to update (newcoming word). To update min, min-priority queue required
        if (words_map.count(word) == 1) {

            int pos_min, word_min;

            // if word is the first word snippet, update the queue and the min_position
            std::tie(pos_min, word_min) = words_queue.top();

            while (words_map.count(word_min) == 1 && pos_min != words_map[word_min]) { // top is useless. Indeed, is somewhere else in the queue, alfter top position

                words_queue.pop();
                std::tie(pos_min, word_min) = words_queue.top();
                min_position = pos_min;
            }
        }
        // if all words in map and better lenght achieved, update best_length
        if (words_map.size() == n && max_position - min_position + 1 < best_length) {
            best_length = max_position - min_position + 1;
        }
        relative_position ++;
    }

    cout << best_length << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}