#include <cstdio>
#include <vector>
#include <queue>

struct edge {
  unsigned int src;
  unsigned int dst;
  bool cap;
  unsigned int index;
  edge(unsigned int src, unsigned int dst, bool cap, unsigned int index): src(src), dst(dst), cap(cap), index(index) {}
};

typedef std::vector<edge> edge_vector;
typedef std::vector<edge_vector> edge_vector_vector;
typedef std::vector<bool> bool_vector;
typedef std::queue<unsigned int> uint_queue;

int main() {
  unsigned int N, K;
  scanf("%u", &N);
  scanf("%u", &K);
  edge_vector_vector out_adj(1 + 2 * N + 1);
  for (unsigned int i1 = 0; i1 < N; ++i1) {
    out_adj[0].push_back(edge(0, 1 + i1, true, out_adj[1 + i1].size()));
    out_adj[1 + i1].push_back(edge(1 + i1, 0, false, out_adj[0].size() - 1));
    out_adj[1 + N + i1].push_back(edge(1 + N + i1, 2 * N + 1, true, out_adj[2 * N + 1].size()));
    out_adj[2 * N + 1].push_back(edge(2 * N + 1, 1 + N + i1, false, out_adj[1 + N + i1].size() - 1));
  }
  for (unsigned int i1 = 0; i1 < K; ++i1) {
    unsigned int R, C;
    scanf("%u", &R);
    scanf("%u", &C);
    out_adj[1 + R - 1].push_back(edge(1 + R - 1, 1 + N + C - 1, true, out_adj[1 + N + C - 1].size()));
    out_adj[1 + N + C - 1].push_back(edge(1 + N + C - 1, 1 + R - 1, false, out_adj[1 + R - 1].size() - 1));
  }
  unsigned int flow = 0;
  bool sink_found = true;
  while (sink_found) {
    sink_found = false;
    uint_queue bfs_queue;
    std::vector<edge *> backtrack_vector(2 * N + 2);
    bool_vector bfs_seen(2 * N + 2, false);
    bfs_queue.push(0);
    backtrack_vector[0] = NULL;
    bfs_seen[0] = true;
    while ((!sink_found) && (!bfs_queue.empty())) {
      unsigned int current = bfs_queue.front();
      bfs_queue.pop();
      for (unsigned int i3 = 0; (!sink_found) && (i3 < out_adj[current].size()); ++i3) {
        if ((!bfs_seen[out_adj[current][i3].dst]) && out_adj[current][i3].cap) {
          if (out_adj[current][i3].dst == 2 * N + 1) {
            sink_found = true;
          }
          bfs_queue.push(out_adj[current][i3].dst);
          backtrack_vector[out_adj[current][i3].dst] = &(out_adj[current][i3]);
          bfs_seen[out_adj[current][i3].dst] = true;
        }
      }
    }
    if (sink_found) {
      for (edge *current = backtrack_vector[2 * N + 1]; current != NULL; current = backtrack_vector[current->src]) {
        current->cap = false;
        out_adj[current->dst][current->index].cap = true;
      }
      ++flow;
    }
  }
  printf("%u\n", flow);
  return 0;
}
