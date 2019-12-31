#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <bits/stdc++.h> // needed for priority queue
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

/*
http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/set.html


2 sols :
Use boost graphs and Dijskstra OR
implement dijkstra from scratch (done here)
careful to not add int to max_int (otherwise negative)
hash pairs with boost
min priority queu class comparator
*/

struct vertex {
    pair<int, int> pos;
    pair<int, int> parent;
    int distance;
    int weight;
};

class myComparator 
{ 
public: 
    int operator() (const vertex& p1, const vertex& p2) 
    { 
        return p1.distance > p2.distance; 
    } 
}; 

void dijkstra(unordered_map<pair<int, int>, vertex, boost::hash<pair<int, int>>>& vertices, 
            vector<vector<vector<pair<int, int>>>>& adj_list) {
    
    // push all vertices to min priority queue
    priority_queue<vertex, vector<vertex>, myComparator> Q;
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
        Q.push((*it).second); // since iterated over map
    }

    // core dijkstra
    while (!Q.empty()) {

        vertex u = Q.top();
        Q.pop();
        int i_u = u.pos.first;
        int j_u = u.pos.second;
        int d_u = u.distance;

        for (auto it = adj_list[i_u][j_u].begin(); it != adj_list[i_u][j_u].end(); ++it) {
            
            int i_v = (*it).first;
            int j_v = (*it).second;
            int d_v = vertices[{i_v, j_v}].distance;
            int w_v = vertices[{i_v, j_v}].weight;

            // relax
            if (d_u < std::numeric_limits<int>::max() && d_u + w_v < d_v) { // careful to not add to a max int
                // cerr << "relax " << i_u << " " << j_u << " " << i_v << " " << j_v << 
                //     " " << d_u + w_v << " " << d_v << "\n";
                vertices[{i_v, j_v}].distance = d_u + w_v;
                Q.push(vertices[{i_v, j_v}]);
            }
        }
    }
}

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2> &pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

void testcase() {

    int k, v;
    cin >> k;

    // create adj list
    vector<vector<vector<pair<int, int>>>> adj_list(k, vector<vector<pair<int, int>>>(k, vector<pair<int, int>>()));
    unordered_map<pair<int, int>, vertex, boost::hash<pair<int, int>>> vertices_a(k);
    unordered_map<pair<int, int>, vertex, boost::hash<pair<int, int>>> vertices_b(k);
    unordered_map<pair<int, int>, vertex, boost::hash<pair<int, int>>> vertices_c(k);
    
    for (int i = 0; i < k; i ++) {
        for (int j = 0; j <= i; j ++) {

            // create vertex
            cin >> v;

            // 7 cases for number of neighbors

            if (i == 0 && j == 0) {

                vertices_a[{i, j}] = {{i, j}, {NULL, NULL}, v, v};
                vertices_b[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};
                vertices_c[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};

                adj_list[i][j].push_back({1, 0});
                adj_list[i][j].push_back({1, 1});
            }
            else if (i == k - 1 && j == 0) {

                vertices_a[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};
                vertices_b[{i, j}] = {{i, j}, {NULL, NULL}, v, v};
                vertices_c[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};

                adj_list[i][j].push_back({k - 2, 0});
                adj_list[i][j].push_back({k - 1, 1});
            }
            else if (i == k - 1 && j == k - 1) {

                vertices_a[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};
                vertices_b[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};
                vertices_c[{i, j}] = {{i, j}, {NULL, NULL}, v, v};

                adj_list[i][j].push_back({k - 2, k - 2});
                adj_list[i][j].push_back({k - 1, k - 2});
            }
            else {

                vertices_a[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};
                vertices_b[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};
                vertices_c[{i, j}] = {{i, j}, {NULL, NULL}, std::numeric_limits<int>::max(), v};


                if (j == 0) {
                    adj_list[i][j].push_back({i - 1, j});
                    adj_list[i][j].push_back({i, j + 1});
                    adj_list[i][j].push_back({i + 1, j + 1});
                    adj_list[i][j].push_back({i + 1, j});
                }
                else if (i == j) {
                    adj_list[i][j].push_back({i - 1, j - 1});
                    adj_list[i][j].push_back({i, j - 1});
                    adj_list[i][j].push_back({i + 1, j});
                    adj_list[i][j].push_back({i + 1, j + 1});
                }
                else if (i == k - 1) {
                    adj_list[i][j].push_back({i, j - 1});
                    adj_list[i][j].push_back({i - 1, j - 1});
                    adj_list[i][j].push_back({i - 1, j});
                    adj_list[i][j].push_back({i, j + 1});
                }
                else {
                    adj_list[i][j].push_back({i - 1, j - 1});
                    adj_list[i][j].push_back({i - 1, j});
                    adj_list[i][j].push_back({i, j + 1});
                    adj_list[i][j].push_back({i + 1, j + 1});
                    adj_list[i][j].push_back({i + 1, j});
                    adj_list[i][j].push_back({i, j - 1});
                }
            }
        }
    }

    // cerr << "adj list: \n";
    // for (int i = 0; i < k; i ++) {
    //     for (int j = 0; j <= i; j ++) {
    //         cerr << i << " " << j << " "  ;
    //         for (auto it = adj_list[i][j].begin(); it != adj_list[i][j].end(); ++it){
    //             cerr << (*it).first << " " << (*it).second << " ";
    //         }
    //         cerr << "\n";
    //     }
    // }

    // run dijkstra from each apex
    //cerr << "DISTANCE A \n";
    dijkstra(vertices_a, adj_list);
    //cerr << "DISTANCE B \n";
    dijkstra(vertices_b, adj_list);
    //cerr << "DISTANCE C \n";
    dijkstra(vertices_c, adj_list);

    // for each candidate center, compute shortest path from each of 3 apices (a, b, c)
    int distance = std::numeric_limits<int>::max();
    for (int i = 0; i < k; i ++) {
        for (int j = 0; j <= i; j ++) {
            int distance_a = vertices_a[{i, j}].distance;
            int distance_b = vertices_b[{i, j}].distance;
            int distance_c = vertices_c[{i, j}].distance;
            distance = min(distance, distance_a + distance_b + distance_c - 2 * vertices_a[{i, j}].weight);
            //cerr << i << " " << j << " " << distance_a << " " << distance_b << " " << distance_c << " " << distance << "\n";
        }
    }

    cout << distance << "\n";
 }

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) testcase();
}
