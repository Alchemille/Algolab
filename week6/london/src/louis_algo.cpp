#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

/*
https://stackoverflow.com/questions/905355/how-to-get-the-number-of-characters-in-a-stdstring
https://stackoverflow.com/questions/5455802/how-to-read-a-complete-line-from-the-user-using-cin
https://www.studytonight.com/cpp/stl/stl-container-map
https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-c-stdmap
https://stackoverflow.com/questions/4324763/can-we-have-functions-inside-functions-in-c
https://stackoverflow.com/questions/26281979/c-loop-through-map

https://stackoverflow.com/questions/11984297/creating-pair-object-c
https://stackoverflow.com/questions/10124679/what-happens-if-i-read-a-maps-value-where-the-key-does-not-exist

Difficulties:
at first, had a different node for each input letter. third inner loop to create middle edges.
Now no third loop and less vertices
tried to reduce #edges by counting #occurences of each letter in note, and have edges of capacity the number of occurences

Tentative 2 : 1 + n + h*w + 1 nodes

Third tentative to reduce edges and vertices:
Not have duplicate vertices in the journal vertices. Before, 1 node per pair.
Instead, have 1 node per letter.
*/

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

using namespace std;

class edge_adder {
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void check_telegraph() {
    // read input. Use letter index for encoding letters instead of char.
    int w, h, n;
    char a;
    cin >> h >> w;
    string str_note;
    cin >> str_note;
    n = str_note.length();
    vector<int> occurences_note(26, 0); // map note character to number of occurences in note
    vector<int> occurrences_journal(676, 0);
    vector<int> recto(h * w);

    graph G(2 + 26 + 676);
    edge_adder adder(G);

    // Count distinct characters in the note and save number of occurrences
    for (int i = 0; i < n; i++) {
        occurences_note[str_note[i] - 'A']++;
    }

    // add edges from source to note
    for (int i = 0; i < 26; i++) {
        if (occurences_note[i] == 0) {
            continue;
        }
        adder.add_edge(0, 2 + i, occurences_note[i]);
    }

    // Parse recto
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cin >> a;
            recto[i * w + j] = a - 'A';
        }
    }

    // Parse verso and create node for every unique pair
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cin >> a;
            int u = min(a - 'A', recto[i * w + w - j - 1]);
            int v = max(a - 'A', recto[i * w + w - j - 1]);
            occurrences_journal[26 * u + v]++;
        }
    }

    // edges from note to distinc pair
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            if (occurences_note[i])
                adder.add_edge(2 + i, 2 + 26 + 26 * i + j, occurences_note[i]);

            if (occurences_note[j])
                adder.add_edge(2 + j, 2 + 26 + 26 * i + j, occurences_note[j]);

            if (occurrences_journal[26 * i + j]) {
                adder.add_edge(2 + 26 + 26 * i + j, 1, occurrences_journal[26 * i + j]);
            }
        }
    }

    // edge_iterator eit1 = edges(G).first;
    // cout << "\n";
    // for (; eit1!=edges(G).second; ++eit1) {
    //     cout << boost::get(boost::edge_capacity, G, *eit1) << " " << source(*eit1, G) << " " << target(*eit1, G) << "\n";
    // }
    // cout << "\n";

    long flow = boost::push_relabel_max_flow(G, 0, 1);
    if (flow == n) cout << "Yes\n";
    else cout << "No\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;

    while (t--) check_telegraph();
}
