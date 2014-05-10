#include <cstdio>

#include <vector>
typedef std::vector<bool> bool_vector;
typedef std::vector<bool_vector> adjacency_matrix;

#include <queue>
typedef std::queue<unsigned int> uint_queue;

typedef std::vector<unsigned int> uint_vector;

#include <cassert>

FILE *fin, *fout;

void dotestcase(unsigned int N, unsigned int M) {
  bool_vector temp1(M + N + 2, false);
  adjacency_matrix out_adj(M + N + 2, temp1);
  for (unsigned int i1 = 1; i1 <= N; ++i1) {
    out_adj[0][i1] = true;
  }
  for (unsigned int i1 = 1; i1 <= N; ++i1) {
    unsigned int Si;
    fscanf(fin, "%u", &Si);
    for (unsigned int i2 = 1; i2 <= Si; ++i2) {
      unsigned int i3;
      fscanf(fin, "%u", &i3);
      out_adj[i1][N + i3] = true;
    }
  }
  for (unsigned int i1 = 1; i1 <= M; ++i1) {
    out_adj[N + i1][M + N + 1] = true;
  }
  unsigned int flow = 0;
  bool done = false;
  while (!done) {
    uint_queue bfs_queue;
    bool_vector bfs_seen(M + N + 2);
    uint_vector bfs_backtracker(M + N + 2);
    bfs_queue.push(1);
    bfs_seen[1 - 1] = true;
    bfs_backtracker[1 - 1] = 0;//none
    while (!bfs_seen[M + N + 2 - 1] && !bfs_queue.empty()) {
      unsigned int current = bfs_queue.front();
      bfs_queue.pop();
      for (unsigned int i1 = 1; i1 <= M + N + 2; ++i1) {
        if (out_adj[current - 1][i1 - 1] && !bfs_seen[i1 - 1]) {
          bfs_queue.push(i1);
          bfs_seen[i1 - 1] = true;
          bfs_backtracker[i1 - 1] = current;
        }
      }
    }
    if (!bfs_seen[M + N + 2 - 1]) {
      done = true;
    } else {
      for (unsigned int current = M + N + 2;
           bfs_backtracker[current - 1] > 0;
           current = bfs_backtracker[current - 1]) {
        assert(out_adj[bfs_backtracker[current - 1] - 1][current - 1] &&
               !out_adj[current - 1][bfs_backtracker[current - 1] - 1]);
        out_adj[bfs_backtracker[current - 1] - 1][current - 1] = false;
        out_adj[current - 1][bfs_backtracker[current - 1] - 1] = true;
      }
      flow += 1;
    }
  }
  fprintf(fout, "%u\n", flow);
}

main() {
  fin = stdin;
  fout = stdout;
  unsigned int N, M;
  while (fscanf(fin, "%u %u", &N, &M) != EOF) {
    dotestcase(N, M);
  }
}
