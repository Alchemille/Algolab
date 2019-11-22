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
        return compare_positions(p1, p2); 
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

    vector<pair<int, int>> all_positions; // a pair is {position of word, word}
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
    words_queue.push(all_positions[0]); // !! careful, position, word unlike map

    int min_position = abs_position;
    int max_position = abs_position;

    int relative_position = 1;
    int best_length = std::numeric_limits<int>::max();
    
    while(relative_position < n-1) {
        
        abs_position = all_positions[relative_position].first;
        word = all_positions[relative_position].second;

        // if word already in map, replace it and update min and max positions.
        // max easy to update (newcoming word). To update min, min-priority queue required
        if (words_map.count(word) == 1) {

            words_map[word] = abs_position;
            int pos_min, word_min;

            // update max position
            max_position = abs_position;

            // if word is the first word snippet, update the queue and the min_position
            std::tie(pos_min, word_min) = words_queue.top();
            if (word_min == word) {
                words_queue.pop();
                words_queue.push({abs_position, word});
                min_position = words_queue.top().first;

                continue; // do not discover new words until we are sure the first word is not the same as the last one
            }
        }
        else { // simply add new word to map and update max position
            words_map[word] = abs_position;
            max_position = abs_position;
        }
        
        // if all words in map and better lenght achieved, update best_length
        if (words_map.size() == n && max_position - min_position + 1 < best_length) {
            best_length = max_position - min_position + 1;
        }

        relative_position ++;
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
