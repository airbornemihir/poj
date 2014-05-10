#include <cstdio>
#include <vector>
typedef std::vector<unsigned int> VU;
typedef std::vector<VU> VVU;
typedef std::vector<bool> VB;
typedef std::vector<VB> VVB;

int main() {
  unsigned int N;
  for (scanf("%u\n", &N); N > 0; scanf("%u\n", &N)) {
    VVU out_adj(N, VU(N)); VVB reachable(N, VB(N, false));
    for (unsigned int i1 = 1; i1 <= N; ++i1) {
      unsigned int out_degree;
      scanf("%u\n", &out_degree);
      for (unsigned int i2= 1; i2 <= out_degree; ++i2) {
        unsigned int temp1;
        scanf("%u", &temp1);
        scanf("%u", &(out_adj[i1 - 1][temp1 - 1]));
        reachable[i1 - 1][temp1 - 1] = true;
      }
      out_adj[i1 - 1][i1 - 1] = 0;
      reachable[i1 - 1][i1 - 1] = true;
    }
    for (unsigned int i1 = 1; i1 <= N; ++i1) {
      for (unsigned int i2 = 1; i2 <= N; ++i2) {
        for (unsigned int i3 = 1; i3 <= N; ++i3) {
          if (reachable[i2 - 1][i1 - 1] &&
              reachable[i1 - 1][i3 - 1] &&
              (!reachable[i2 - 1][i3 - 1] ||
               (out_adj[i2 - 1][i3 - 1] >
                out_adj[i2 - 1][i1 - 1] + out_adj[i1 - 1][i3 - 1]))) {
            out_adj[i2 - 1][i3 - 1] =
              out_adj[i2 - 1][i1 - 1] + out_adj[i1 - 1][i3 - 1];
            reachable[i2 - 1][i3 - 1] = true;
          }
        }
      } 
    }
    bool is_disjoint = true;
    unsigned int opt1, opt_i1;
    for (unsigned int i1 = 1; i1 <= N; ++i1) {
      bool is_connected = true;
      unsigned int opt2 = 0;
      unsigned int i2;
      for (i2 = 1; (i2 <= N) && is_connected; ++i2) {
        if ((is_connected = reachable[i1 - 1][i2 - 1]) && (opt2 < out_adj[i1 - 1][i2 - 1])) {
          opt2 = out_adj[i1 - 1][i2 - 1];
        } 
      }
      if (is_connected) {
        if (is_disjoint || (opt1 > opt2)) {
          opt1 = opt2;
          opt_i1 = i1;
        }
        is_disjoint = false;
      }
    }
    if (is_disjoint) {
      printf("disjoint\n");
    } else {
      printf("%u %u\n", opt_i1, opt1);
    }
    
  }
  return 0;
}
