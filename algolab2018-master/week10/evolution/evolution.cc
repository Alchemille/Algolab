#include <bits/stdc++.h>

struct Query{
	int idx;
	int b;
};

struct A{
	int age;
	int idx;
	const bool operator<(A n2) const{
		return age > n2.age;
	}
};

struct Node{		
	Node* parent;
	std::vector<Node*> children;
	std::vector<Query> queries;
	int age;
	int idx;
	std::string name;
	Node(){
		age = 0;
		parent = NULL;
		name = "";
	}
	Node copy(){
		Node n;
		n.age = age;
		n.name = name;
		n.parent = parent;
		return n;
	}
	const bool operator<(Node n2) const{
		return age > n2.age;
	}
};

struct Tree{
	std::vector<Node> nodes;
	std::unordered_map<std::string, int> name_to_idx;
	int root;

	void compute_root(){
		for(int i=0; i<nodes.size(); i++){
			if(nodes[i].parent == NULL)
				root = i;
		}
	}

	int BS(std::vector<A> &line, int b){
		A n2;
		n2.age = b;
		n2.idx = 0;
		return std::lower_bound(line.begin(), line.end(), n2)->idx;
	}

	void DFS(std::stack<int> &S, std::vector<A> &current_line, std::vector<int> &queries_results){
		int curr = S.top();
		S.pop();
		// answer queries of the curr node if any
		std::vector<Query> queries = nodes[curr].queries;
		for(Query &q: queries){
			int ans = BS(current_line, q.b);
			queries_results[q.idx] = ans;
		}
		// Need to do that cause there's no deep copy in the Node struct
		std::vector<Node*> children = nodes[curr].children;
		for(Node* n: children){
			current_line.push_back({n->age, n->idx});
			S.push(n->idx);
			DFS(S, current_line, queries_results);
		}
		current_line.pop_back();
	}
};

void testcase(){
	int n, q;
	std::cin >> n >> q;

	Tree t;
	t.nodes = std::vector<Node>(n, Node());
	t.name_to_idx = std::unordered_map<std::string, int>();
	
	for(int i=0; i<n; i++){
		std::string name; std::cin >> name;
		int age; std::cin >> age;
		t.name_to_idx[name] = i;
		t.nodes[i].age = age;
		t.nodes[i].name = name;
		t.nodes[i].idx = i;
	}
	for(int i=0; i<n-1; i++){
		std::string s, p; std::cin >> s >> p;
		t.nodes[t.name_to_idx[s]].parent = &t.nodes[t.name_to_idx[p]];
		t.nodes[t.name_to_idx[p]].children.push_back(&t.nodes[t.name_to_idx[s]]);
	}
	std::vector<int> queries_results(q);

	for(int i=0; i<q; i++){
		std::string s; std::cin >> s;
		int b; std::cin >> b;
		Query query;
		query.idx = i; query.b = b;
		t.nodes[t.name_to_idx[s]].queries.push_back(query);
	}
	t.compute_root();

	std::stack<int> S; S.push(t.root);
	std::vector<A> curr_line; curr_line.push_back({t.nodes[t.root].age, t.root});

	t.DFS(S, curr_line, queries_results);
	for(auto &s: queries_results){
		std::cout << t.nodes[s].name << " ";
	}

	std::cout << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);	
	int T; std::cin >> T;
	for(int t=0; t<T; t++)
		testcase();
	return 0;
}
