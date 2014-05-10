#include <cstdio>
struct edge {
  unsigned int src;
  unsigned int dst;
  unsigned int weight;
  edge(unsigned int src,
       unsigned int dst,
       unsigned int weight):
    src(src),
    dst(dst),
    weight(weight) {}
};
#include <map>
typedef std::multimap<unsigned int, edge> weight_edge_map;
#include <vector>
typedef std::vector<bool> VB;
typedef std::vector<unsigned int> VU;
#include <cassert>

int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);

  unsigned int N, A, B;
  scanf("%u", &N);
  scanf("%u", &A);
  scanf("%u", &B);
  weight_edge_map m;
  for (unsigned int i1 = 1; i1 <= N; ++i1) {
    unsigned int Ki;
    scanf("%u", &Ki);
    unsigned int src = i1, dst, weight;
    for (unsigned int i2 = 1; i2 <= Ki; ++i2) {
      scanf("%u", &dst);
      weight = (i2 <= 1) ? 0 : 1;
      m.insert(weight_edge_map::value_type(weight, edge(src, dst, weight)));
    }
  }
  VB seen(N, false);
  std::vector<edge *> backtracker(N, NULL);
  
  seen[A - 1] = true;
  backtracker[A - 1] = NULL;
  weight_edge_map::iterator i1 = m.begin();
  while ((!seen[B - 1]) && (i1 != m.end())) {
    if (seen[i1->second.src - 1]) {
      if (!seen[i1->second.dst - 1]) {
	seen[i1->second.dst - 1] = true;
	backtracker[i1->second.dst - 1] =
	  new edge(i1->second.src,
		   i1->second.dst,
		   i1->second.weight);
      }
      m.erase(i1);
      i1 = m.begin();
    } else {
      ++i1;
    }
  }
  if (seen[B - 1]) {
    unsigned int cost = 0;

    for (unsigned int i1 = B;
	 backtracker[i1 - 1] != NULL;
	 i1 = backtracker[i1 - 1]->src) {
      cost += backtracker[i1 - 1]->weight;
    }
    printf("%u\n", cost);
  } else {
    printf("-1\n");
  }
  // fclose(stdout);
  // fclose(stdin);
  return 0;
}
