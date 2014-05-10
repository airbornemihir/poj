#include <cstdio>
#include <map>
#include <vector>
#include <cassert>

struct edge {
  unsigned int src;
  unsigned int dst;
  unsigned int weight;
  edge(unsigned int src, unsigned int dst, unsigned int weight): src(src), dst(dst), weight(weight) {}
};

typedef std::multimap<unsigned int, edge> weight_edge_map;
typedef std::vector<bool> bool_vector;

int main() {
  unsigned int N, M;
  scanf("%u", &N);
  scanf("%u", &M);
  weight_edge_map w;
  for (unsigned int i1 = 0; i1 < M; ++i1) {
    unsigned int src;
    unsigned int dst;
    unsigned int weight;
    scanf("%u", &src);
    scanf("%u", &dst);
    scanf("%u", &weight);
    edge e(src, dst, weight);
    w.insert(weight_edge_map::value_type(weight, e));
  }
  bool_vector seen(N, false);
  unsigned int seen_count = 0;
  seen[1 - 1] = true;
  ++seen_count;
  unsigned int max = 0;
  while (seen_count < N) {
    bool found = false;
    for (weight_edge_map::iterator i1 = w.begin(); !found; ++i1) {
      if (seen[i1->second.src - 1]) {
        if (!seen[i1->second.dst - 1]) {
          seen[i1->second.dst - 1] = true;
          found = true;
        }
      } else {
        if (seen[i1->second.dst - 1]) {
          seen[i1->second.src - 1] = true;
          found = true;
        }
      }
      if (found) {
        ++seen_count;
        max = max >= i1->first ? max : i1 -> first;
      }
    }
  }
  printf("%u\n", max);
  return 0;
}
