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

// function from slides.
void add_edge(int from, int to, long capacity, graph& G) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

void check_telegraph() {

    // read input. Use letter index for encoding letters instead of char.
    int w, h, n, distinct_chars, distinct_pairs, u, v;
    char a;
    cin >> h >> w;
    string str_note;
    cin >> str_note;
    n = str_note.length();
    map<char, int> map_note; // map note character to node number in graph
    map<char, int> occurences_note; // map note character to number of occurences in note
    map<char, int> map_journal;
    map<char, int> occurrences_journal;
    vector<char> recto(h*w);
    vector<char> verso(h*w);

    graph G(2);

    // Count distinct characters in the note and save number of occurrences
    distinct_chars = 2;
    for (int i=0; i<n; i++) {
        if (map_note.count(str_note[i]) == 0) {
            map_note[str_note[i]] = distinct_chars; // + 1 because node 0 is source
            distinct_chars ++;
            occurences_note[str_note[i]] = 0;
        }
        occurences_note[str_note[i]] ++;
    }

    // add edges from source to note
    for (map<char, int>::iterator it = occurences_note.begin(); it!=occurences_note.end(); ++it) {
        add_edge(0, map_note[(*it).first], (*it).second, G);
    }

    // Count distinct characters in the note and save number of occurrences
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            cin >> a;
            if (map_journal.count(a) ==  0) {
                map_journal[a] = distinct_chars;
                distinct_chars ++;
            }
            recto[i*w+j] = a;
        }
    }

    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            cin >> a;
            if (map_journal.count(a) ==  0) {
                map_journal[a] = distinct_chars;
                distinct_chars ++;
            }
            verso[i*w+j] = a;
        }
    }

    // edges from note to journal to its representant in journal
    for (map<char, int>::iterator it = map_note.begin(); it!=map_note.end(); ++it) {
        if (map_journal.count((*it).first) == 0) {
            cout << "No\n";
            return;
        }
        else {
            u = (*it).second;
            v = map_journal[(*it).first];
            edge_desc e = boost::edge(0, u, G).first;
            auto c = boost::get(boost::edge_capacity, G, boost::edge(0, u, G).first);
            add_edge(u, v, c, G);
        }
    }

    // edges from a journal node to its acolyte, from small to big (according to map_journal ordering)
    auto cap_map = boost::get(boost::edge_capacity, G);
    auto rev_map = boost::get(boost::edge_reverse, G);
    edge_desc e;
    bool s;
    distinct_pairs = 0;
    map<edge_desc, int> distinct_pairs_vertex;

    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {

            int u = map_journal[recto[i*w+j]];
            int v = map_journal[verso[i*w + w -j -1]];

            boost::tie(e, s) = boost::edge(min(u, v), max(u, v), G);

            if (s) {
                //cout << "1 " << cap_map[e] << "\n";
                cap_map[e] ++;
                int added_vertex = distinct_pairs_vertex[e];
                boost::tie(e, s) = boost::edge(max(u, v), added_vertex, G);
                cap_map[e] ++;
                boost::tie(e, s) = boost::edge(added_vertex, min(u, v), G);
                cap_map[e] ++;                
                boost::tie(e, s) = boost::edge(added_vertex, 1, G);
                cap_map[e] ++;
            }

            else {
                //cout << min(u, v) << "\n";
                add_edge(min(u, v), max(u, v), 1, G);
                add_edge(max(u, v), distinct_chars + distinct_pairs, 1, G);
                add_edge(distinct_chars + distinct_pairs, min(u, v), 1, G);
                add_edge(distinct_chars + distinct_pairs, 1, 1, G);
                edge_desc e = boost::edge(min(u, v), max(u, v), G).first;
                distinct_pairs_vertex[e] = distinct_chars + distinct_pairs;
                distinct_pairs ++;
            }
        }
    }

    // edge_iterator eit1 = edges(G).first;
    // cout << "\n";
    // for (; eit1!=edges(G).second; ++eit1) {
    //     if (boost::get(boost::edge_capacity, G, *eit1) ) cout << boost::get(boost::edge_capacity, G, *eit1) << " " << source(*eit1, G) << " " << target(*eit1, G) << "\n";
    // }
    // cout << "\n";

    long flow = boost::push_relabel_max_flow(G, 0, 1);
    if (flow == n) cout << "Yes\n";
    else cout << "No\n";
    //cout << flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;

    while(t--) check_telegraph();
}
