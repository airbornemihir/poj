#include <iostream>
#include <string>
#include <cassert>
#include <vector>
typedef std::vector<unsigned int> uint_vector;
typedef std::vector<uint_vector> uint_vector_vector;
#include <algorithm>

int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);

  unsigned int N;
  std::cin >> N;
  std::string s;
  std::cin >> s;
  assert(N == s.size());

  uint_vector_vector m(2);
  for (unsigned int i1 = 0; i1 <= N; ++i1) { //iterates over lengths
    if (i1 <= 1) {
      for (unsigned int i2 = 0; i2 <= N - i1; ++i2) {
	m[i1].push_back(0);
      }
      assert(m[i1].size() == N + 1 - i1);
    } else {
      uint_vector temp = m[0];
      m[0] = m[1];
      m[1].clear();
      for (unsigned int i2 = 0; i2 <= N - i1; ++i2) {
	if (s[i2] == s[i2 + i1 - 1]) {
	  m[1].push_back(temp[i2 + 1]);
	} else {
	  m[1].push_back(1 + std::min(m[0][i2], m[0][i2 + 1]));
	}
      }
      assert(m[1].size() == N + 1 - i1);
    }
  }
  std::cout << m[1][1 - 1] << std::endl;
  // fclose(stdin);
  // fclose(stdout);
  return 0;
}
