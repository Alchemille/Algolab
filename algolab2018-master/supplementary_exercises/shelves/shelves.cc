#include <bits/stdc++.h>

void testcase(){
	int l; std::cin >> l;
	int m, n; std::cin >> m >> n;

	int bm = 0, bn = 0, bw = l;
	int nm = 0;

	while(l > 0){
		// try to fit as many n-length shelves as possible
		int nn = l / n;
		int remain = l % n;
		if(remain < bw){
			bm = nm;
			bn = nn;
			bw = remain;
		}
		l -= m;
		nm++;
	}
	std::cout << bm << " " << bn << " " << bw << "\n";
}

int main(){
	std::ios_base::sync_with_stdio(false);
	int N; std::cin >> N;
	for(int i=0; i<N; i++)
		testcase();
	return 0;
}
