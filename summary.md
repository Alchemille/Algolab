# TECHNIQUES

## BIG NONOS

+ vector<P> v(n); cin >> v[i];
Instead, read x and y separately
+ vector<int> v(n); v.push_back(x)
Will end up with vector of size 2*n

## week 1

## week 2

## week 3 CGAL

### almost antenna 

+ Trick support points with minimum circle (at most 3)

### first hit

+ if inter is segment, use colinear... to know how points are ordered
+ optimization to avoid checking all (ray, segment) intersections: maintain a target on the ray with
(source, target) as small as possible and only check (opt segment, segment) intersection

### from Russia with Love

## week 4 BGL

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

### chariot race

+ vicious triple DP
+ 2 recurrences work (mine and Afra's)

## week 5

### punch

### boats

+ in old version, min heap of struct - cause need to repush elements differently
+ structure that contains method

### planks

+ used remove erase idiom 
+ vector insert
+ save DP decisions
+ TODO: get 100pts with split and list

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

### motorcycles
+ when sorting, make sure its consistent
+ use K::RT to store slopes
+ slope = ray.direction().dy() / ray.direction().dx();
+ to make persistent change on vector of struct : 
either do moto[i].prop = ... OR
motorcycle* m = &input_motos[i];
m.prop = ...
BUT NOT
motorcycle m = input_motos[i]; // COPY!
m.prop = ...
+ use multiprecision int128_t with #include <boost/multiprecision/cpp_int.hpp>


## week 6 flows

For indexes, either use map and counter, or the provided structure,
eg alphabet in London or the times in car sharing
Sometimes source and sinks are part of the objects

### cointossing

+ similar to football tournament in slides

### london

+ sol1, at most 26 + 26 * 26 nodes: node for distinct pair. Pairs indexed with a map.
+ sol2, at most 26 + 26 * 26 nodes: use structure given by alphabet. Pairs indexed with ascii.
+ sol3, at most 26 + 26 nodes: Each letter of note represented once. For each pair, link min to max, and max to sink.

### shopping

### tetris

### planet express

+ first idea use strong_components and dijkstra for all pairs
+ then use of strong_components and floyd_warshall from boost, better for all pairs
+ actually, only need all sources single destination paths, so dijkstra ok.
can use master source trick since only interested in shortest path to single dest - dont care about the warehouse that allows the shortest path.
+ interesting methods: make_reverse_graph(G) and print_graph(G, get(prop, G))
https://www.boost.org/doc/libs/1_48_0/libs/graph/doc/reverse_graph.html

## week 7 LP

### inball

Like EPFL disopt

### radiation

+ Use binary search over LP.
+ revelation tricky middle Binary Search
+ lp.set_r(constraint, CGAL::LARGER)
+ Gmpzf as IT and ET

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