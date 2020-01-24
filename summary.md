# TECHNIQUES

## BIG NONOS

+ vector<P> v(n); cin >> v[i];
Instead, read x and y separately
+ vector<int> v(n); v.push_back(x)
Will end up with vector of size 2*n

## week 4

### first steps

+ use of kruskal_minimum_spanning_tree and store mst edges

### shortest path

+ use of edge_index property
+ dijskstra with predicessor map
+ method max_element()

### ant challenge

+ transfer an edge from G1 to edge of G2: use source() and target()

### buddy selection

+ sol1 : map pair(stud1, stud2) to count_commons: unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> commons;
+ include : <boost/functional/hash.hpp>
+ use edmonds_maximum_cardinality_matching
+ sol2 (official) : sort interests of each student, then sliding window

## week 5

### punch

### boats

+ in old version, min heap of struct - cause need to repush elements differently
+ structure that contains method

### planks

+ used remove erase idiom 
+ vector insert
+ save DP decisions

### octopussy

+ DP : child is subproblem of parent
+ propagation of info from parent to child in both solutions

### light museum

+ DP for m = 1
+ algo split and list for m > 1. Was pretty streneous but it works! :D
+ sorted vector of vectors
+ used a lot i << n and & 
+ used lower_bound for binary search
+ algo bruteforce for m > 1:
- interesting use of bit shifting for testing 2^subsets
- int1 & int2 does boolean comparison
- also use of dynamic bitsets 

### goldenEye

+ use K::FT when dealing with distances
+ first idea, in n*m : 
First compute mst over triangulation
For each mission, commpute bfs over mst and save the biggest edge
+ tentative 2, also n*m :
sort edges
answer qu1: union_find based on p, and see whether s and t in same CC
store results of qu1.
compute mst edges which are interesting values to answer qu2 and qu3 (udea if joining 2 CC).
For each mission, define a union-find. Add mst edge one by one (already sorted), and check if s and t in same component

+ final sol :
When looping over missions above, no need to redifine the union-find for each mission. 
Since qu2 and qu3 results are common to all missions.