#include <cstdio>
#include <vector>
typedef std::vector<unsigned int> VU;
typedef std::vector<VU> VVU;
#include <utility>
typedef std::pair<bool, unsigned int> marked_uint;
#include <stack>
typedef std::stack<marked_uint> marked_uint_stack;
typedef std::vector<bool> VB;
struct edge {
  unsigned int src;
  unsigned int dst;
  unsigned int src_size;
  unsigned int dst_size;
  edge() {}
  edge(unsigned int src,
       unsigned int dst,
       unsigned int src_size,
       unsigned int dst_size):
    src(src),
    dst(dst),
    src_size(src_size),
    dst_size(dst_size)
  {}
};
typedef std::vector<edge> VE;
#include <cassert>
#include <climits>

void dotestcase() {
  unsigned int N;
  scanf("%u", &N);
  VE edges(2 * N - 2);
  VVU out_adj(N);
  for (unsigned int i1 = 0; i1 < N - 1; ++i1) {
    unsigned int temp1, temp2;
    scanf("%u", &temp1);
    scanf("%u", &temp2);
    edges[2 * i1].src = temp1;
    edges[2 * i1].dst = temp2;
    edges[2 * i1 + 1].src = temp2;
    edges[2 * i1 + 1].dst = temp1;
    out_adj[temp1 - 1].push_back(2 * i1);
    out_adj[temp2 - 1].push_back(2 * i1 + 1);
  }
  marked_uint_stack s;
  VB dfs_seen(N, false);
  VU subtree_size(N);
  VU departure_time(N);
  s.push(marked_uint(false, 1)); //node number 1 becomes root of
				 //dfs_tree.
  dfs_seen[1 - 1] = true;
  unsigned int departure_counter = 0;
  while (!s.empty()) {
    marked_uint current = s.top();
    s.pop();
    if (current.first) {
      departure_time[current.second - 1] = ++departure_counter;
      subtree_size[current.second - 1] = 1;
      for (unsigned int i1 = 0; i1 < out_adj[current.second - 1].size(); ++i1) {
	edge &e1 = edges[out_adj[current.second - 1][i1]];
	edge &e2 = (out_adj[current.second - 1][i1] % 2 > 0) ? edges[out_adj[current.second - 1][i1] - 1] : edges[out_adj[current.second - 1][i1] + 1];
	assert(current.second == e1.src);
	if (departure_time[current.second - 1] > departure_time[e1.dst - 1]) {
	  subtree_size[current.second - 1] += subtree_size[e1.dst - 1];
	  e1.src_size = N - subtree_size[e1.dst - 1];
	  e1.dst_size = subtree_size[e1.dst - 1];
	  e2.dst_size = N - subtree_size[e1.dst - 1];
	  e2.src_size = subtree_size[e1.dst - 1];
	}
      }
    } else {
      current.first = true;
      s.push(current);
      for (unsigned int i1 = 0; i1 < out_adj[current.second - 1].size(); ++i1) {
	assert(current.second == edges[out_adj[current.second - 1][i1]].src);
	if (!dfs_seen[edges[out_adj[current.second - 1][i1]].dst - 1]) {
	  s.push(marked_uint(false, edges[out_adj[current.second - 1][i1]].dst));
	  dfs_seen[edges[out_adj[current.second - 1][i1]].dst - 1] = true;
	}
      }
    }
  }
  unsigned int opt_i1, opt = UINT_MAX;
  for (unsigned int i1 = 1; i1 <= N; ++i1) {
    unsigned int opt_for_i1 = 0;
    unsigned int remainder_i1 = N - 1;
    for (unsigned int i2 = 0; i2 < out_adj[i1 - 1].size(); ++i2) {
      edge &e1 = edges[out_adj[i1 - 1][i2]];
      if (departure_time[i1 - 1] > departure_time[e1.dst - 1]) {
	if (opt_for_i1 < subtree_size[e1.dst - 1]) {
	  opt_for_i1 = subtree_size[e1.dst - 1];
	}
	remainder_i1 -= subtree_size[e1.dst - 1];
      }
    }
    if (opt_for_i1 < remainder_i1) {
      opt_for_i1 = remainder_i1;
    }
    if (opt > opt_for_i1) {
      opt = opt_for_i1;
      opt_i1 = i1;
    }
  }
  printf("%u %u \n", opt_i1, opt);
}

int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);

  unsigned int t;
  scanf("%u", &t);
  for (unsigned int i1 = 0; i1 < t; ++i1) {
    dotestcase();
  }

  // fclose(stdout);
  // fclose(stdin);
  return 0;
}
