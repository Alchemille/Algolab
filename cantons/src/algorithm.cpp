#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

using namespace std;

/*
idea:
- bipartite graph Jobs VS Zones
- edges source to jobs: revenue
- edges zones to sink: cost to pay
- edges jobs to zones: infinite
Minimum cut corresponds to the cost to pay + the revenues not considered
S is for jobs and zones kept. T for jobs and zones given up.
It cannot cross the infinite edges that go from jobs in S to zones in T.
This means that all the zones required for a kept jobs (in S) are also in S.

Would not work if bipartite graph was from zones to jobs instead of jobs to zones

*/

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

void testcase() {
    int z, j, sum_rewards = 0;
    cin >> z >> j;
    
    // read costs and rewards
    vector<int> costs_zones(z);
    vector<int> reward_jobs(j);

    for (int i=0; i<z; i++) {
        cin >> costs_zones[i];
    }
    for (int i=0; i<j; i++) {
        cin >> reward_jobs[i];
        sum_rewards += reward_jobs[i];
    }

    // initilize graph
    graph G(j + z + 2);

    // add edges from source to jobs
    for (int i=0; i<j; i++) {
        add_edge(0, i+1, reward_jobs[i], G);
    }

    // add edges from zones to sink
    for (int i=0; i<z; i++) {
        add_edge(j+i+1, 1+j+z, costs_zones[i], G);
    }

    // read links from jobs to zones and create edges
    int inf = std::numeric_limits<int>::max();
    for (int i=0; i<j; i++) {
        int n_zones_i;
        cin >> n_zones_i;

        for (int k=0; k<n_zones_i; k++) {
            int zone;
            cin >> zone;

            // create edge from job i to zone
            add_edge(1+i, 1+j+zone, inf, G);
        }
    }

    // compute max flow = min cut
    long min_cut = boost::push_relabel_max_flow(G, 0, 1+j+z);
    long profit = sum_rewards - min_cut;
    cout << profit <<  "\n";
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    
    int t;
    cin >> t;
    for (int i=0; i<t; i++) testcase();

}
