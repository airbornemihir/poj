#include <cstdio>
#include <vector>
typedef std::vector<int> int_vector;
typedef std::vector<int_vector> int_vector_vector;
#include <utility>
typedef std::pair<int, int> int_pair;
#include <queue>
typedef std::queue<int_pair> int_pair_queue;
typedef std::vector<bool> bool_vector;
typedef std::vector<bool_vector> bool_vector_vector;

int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);

  int M, N;
  scanf("%d", &M);
  scanf("%d", &N);
  int_vector_vector g(M, int_vector(N));
  for (int i1 = 0; i1 < M; ++i1) {
    for (int i2 = 0; i2 < N; ++i2) {
      scanf("%d", &(g[i1][i2]));
    }
  }
  bool_vector_vector seen(M, bool_vector(N, false));
  int component_count = 0, max_component_size = 0;
  for (int i1 = 0; i1 < M; ++i1) {
    for (int i2 = 0; i2 < N; ++i2) {
      if (!seen[i1][i2]) {
	++component_count;
	int component_size = 0;
	int_pair_queue bfs_queue;
	bfs_queue.push(int_pair(i1, i2));
	seen[i1][i2] = true;
	while (!bfs_queue.empty()) {
	  int_pair current = bfs_queue.front();
	  bfs_queue.pop();
	  ++component_size;
	  if (!(g[current.first][current.second] & 1) &&
	      (!seen[current.first][current.second - 1])) {
	    bfs_queue.push(int_pair(current.first, current.second - 1));
	    seen[current.first][current.second - 1] = true;
	  }
	  if (!(g[current.first][current.second] & 2) &&
	      (!seen[current.first - 1][current.second])) {
	    bfs_queue.push(int_pair(current.first - 1, current.second));
	    seen[current.first - 1][current.second] = true;
	  }
	  if (!(g[current.first][current.second] & 4) &&
	      (!seen[current.first][current.second + 1])) {
	    bfs_queue.push(int_pair(current.first, current.second + 1));
	    seen[current.first][current.second + 1] = true;
	  }
	  if (!(g[current.first][current.second] & 8) &&
	      (!seen[current.first + 1][current.second])) {
	    bfs_queue.push(int_pair(current.first + 1, current.second));
	    seen[current.first + 1][current.second] = true;
	  }
	}
	if (max_component_size < component_size)
	  max_component_size = component_size;
      }
    }
  }
  printf("%d\n%d\n", component_count, max_component_size);

  // fclose(stdout);
  // fclose(stdin);
  return 0;
}
