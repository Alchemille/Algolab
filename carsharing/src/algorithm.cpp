#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

/*
Idea for when 2 stations and time is discret (multiple of 30) = DP
Each car has a separate DP with all the available missions
Kinda greedy: the first car can choose in all missions, the second car has less...
Recursion: T[station][time] = max_over_accessible_from_station(profit_mission + T[other_station][arrival])

Cas general: min cost max flow

- Tried to control the numer of vertices: First add all vertices of station 0, then all of station 1... etc...
- Actually, no need since number of times is 10^6.
In sol, simpler and better to use boolean over matrix (S * num_times) that registers vertex_index if it exists for this (station, time)
To add waiting edges for a station, simply loop over 10^6 times and check if was used.

Remove negative costs:
We cant just add constant to each cost, otherwise we could have
s1 -> s2 : -5 + 100 , 
s2 -> s3 : - 5 + 100,  
s1 -> s2 : -10 + 100 loses but should win
We have to make sure the same amount of constants (10000 * 100) for example, is added to all units of flow.
Key is to use the times! since there are 10000 times, the added weights are distributed easily
Works because each path must use exactly 10000seconds !

Technical:
https://www.geeksforgeeks.org/last-element-of-vector-in-cpp-accessing-and-updating/
https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
rbegin() is the reverse of begin()
*/

using namespace std;

struct request {
    int start;
    int target;
    int departure;
    int arrival;
    int profit;
};

void testcase() {

    int n, s;
    cin >> n >> s;
    vector<int> cars(s);

    for (int i = 0; i < s; i ++) {
        cin >> cars[i];
    }

    vector<vector<int>> tmp_stations_times(s, vector<int>());
    vector<request> queries(n);

    for (int i = 0; i < n; i ++) {

        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        queries[i] = {s-1, t-1, d, a, p};
        tmp_stations_times[s-1].push_back(d);
        tmp_stations_times[t-1].push_back(a);
    }

    // build graph
    graph G(n);
    edge_adder adder(G);
    int vertices_cpt = 0;
    
    // add waiting edges between alike stations at different timing
    vector<unordered_map<int, int>> stations_times(s); // for each station, map relevant time to vertex index

    for (int i = 0; i < s; i ++) {

        int num_duplicate_station = tmp_stations_times[i].size();
        sort(tmp_stations_times[i].begin(), tmp_stations_times[i].end());
        tmp_stations_times[i].erase(unique(tmp_stations_times[i].begin(), tmp_stations_times[i].end()), tmp_stations_times[i].end());

        for (int j = 0; j < num_duplicate_station; j ++) {
            stations_times[i][stations_times[i][j]] = vertices_cpt + j;
            if (j < num_duplicate_station - 1) {
                adder.add_edge(vertices_cpt + j, vertices_cpt + j + 1, INT32_MAX, 0);
            }
        }
        vertices_cpt += num_duplicate_station;
    }

    // add travel edges
    for (auto query : queries) {

        int vertex_s = stations_times[query.start][query.departure];
        int vertex_t = stations_times[query.target][query.arrival];

        adder.add_edge(vertex_s, vertex_t, 1, -query.profit);
    }

    // add car edges from source and sink edges
    int source = vertices_cpt ++;
    int sink = vertices_cpt ++;

    for (int i = 0; i < s; i ++) {
        int source_station = stations_times[i][tmp_stations_times[i].back()];
        int sink_station = stations_times[i][tmp_stations_times[i][0]];
        adder.add_edge(source, source_station, cars[i], 0);
        adder.add_edge(sink_station, sink, INT32_MAX, 0);
    }

    // compute min cost max flow
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    int flow = boost::push_relabel_max_flow(G, source, sink);
    boost::cycle_canceling(G);
    int cost = boost::find_flow_cost(G);

    // boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    // int cost = boost::find_flow_cost(G);
    // long flow = 0;
    // for (auto it = boost::out_edges(source, G).first; it != boost::out_edges(source, G).second; ++it) {
    //     flow += c_map[*it] - rc_map[*it];
    // }

    cout << cost << " " << flow << "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t --) testcase();
}
