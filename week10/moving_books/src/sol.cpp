#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int UP_TIME = 1;
const int DOWN_TIME = 2;

bool possibleInKRounds(int k, std::vector<int> &w, std::vector<int> &s) {
  int n = s.size(), m = w.size();
  int i = n - 1, j = m - 1;
  while (i >= 0 && j >= 0) {
    if (s[i] >= w[j]) {
      i -= 1;
      j -= k;
    }
    else {
      return false;
    }
  }
  return (j < 0);
}

int doTest() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> s(n), w(m);
  for (int i = 0 ; i < n ; ++i) {
    std::cin >> s[i];
  }
  for (int i = 0 ; i < m ; ++i) {
    std::cin >> w[i];
  }
  std::sort(s.begin(), s.end());
  std::sort(w.begin(), w.end());
  if (s.back() < w.back()) {
    return -1;
  }
  // in all other cases this is possible
  // here need to bin search the right number of rounds; 
  int left = 1, right = 2;
  while (!possibleInKRounds(right, w, s)) {
    right *= 2;
  }
  while (left < right) {
    int middle = (left + right) / 2;
    if (!possibleInKRounds(middle, w, s)) {
      left = middle + 1;
    } else {
      right = middle;
    }
  }
  /* std::cout << right << "\n"; */
  return left * (UP_TIME + DOWN_TIME) - UP_TIME;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  unsigned test_count;
  std::cin >> test_count;
  for (unsigned i = 0 ; i < test_count ; ++i) {
    int res = doTest();
    if (res < 0) {
      std::cout << "impossible\n";
    }
    else {
      std::cout << res << "\n";
    }
  }
  return 0;
}