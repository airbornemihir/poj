#include <cstdio>
#include <vector>
typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

struct cell {
  VI previous;
  int current_length;
  cell() {}
  cell(int previous_0,
    int previous_1,
    int current_length):
    previous(2), current_length(current_length){
    previous[0] = previous_0;
    previous[1] = previous_1;
  }
};
typedef std::vector<cell> VC;
typedef std::vector<VC> VVC;

#include <utility>
#include <stack>
typedef std::stack<std::pair<int, int> > int_pair_stack;

#include <cassert>

int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);
  VI M(2);
  VVI seq(2);
  for (unsigned int i1 = 0; i1 < 2; ++i1) {
    scanf("%d", &(M[i1]));
    seq[i1].resize(M[i1]);
    // printf("seq[%d]: ", i1);
    for (unsigned int i2 = 0; i2 < M[i1]; ++i2) {
      scanf("%d", &(seq[i1][i2]));
      // printf("%d ", seq[i1][i2]);
    }
    // printf("\n");
  }
  VI B(M[1]), C(M[1]);
  VVC cells(M[0], VC(M[1]));
  int opt = 0, opt_i1, opt_i2;
  for (unsigned int i1 = 0; i1 < M[0]; ++i1) {
    int best_column, best_column_length = 0;
    for (unsigned int i2 = 0; i2 < M[1]; ++i2) {
      // if (i1 == 3 && (i2 == 6 || i2 == 3)) {
      // 	printf("i1 = %d, i2 = %d, best_column = %d, best_column_length = %d\n", i1, i2, best_column, best_column_length);
      // 	printf("B[i2 - 1] = %d, C[i2 - 1] = %d\n", B[i2 - 1], C[i2 - 1]);
      // 	printf("seq[1][i2 - 1] = %d, seq[1][i2] = %d\n", seq[1][i2 - 1], seq[1][i2]);
      // }
      if ((i1 > 0) &&
	  (i2 > 0) &&
	  (best_column_length < B[i2 - 1]) &&
	  (C[i2 - 1] < i1) &&
	  (seq[1][i2 - 1] < seq[0][i1])) {
	best_column_length = B[i2 - 1];
	best_column = i2 - 1;
      }
      // if (i1 == 3 && (i2 == 6 || i2 == 3)) {
      // 	printf("i1 = %d, i2 = %d, best_column = %d, best_column_length = %d\n", i1, i2, best_column, best_column_length);
      // }
      if (seq[0][i1] == seq[1][i2]) {
	cells[i1][i2].current_length = 1;
	if ((i1 > 0) &&
	    (i2 > 0) &&
	    (cells[i1][i2].current_length < best_column_length + 1)) {
	  cells[i1][i2] = cell(C[best_column],
			    best_column,
			    best_column_length + 1);
	}
      } else {
	cells[i1][i2].current_length = 0;
      }
      if ((i1 <= 0) || (B[i2] < cells[i1][i2].current_length)) {
	B[i2] = cells[i1][i2].current_length;
	C[i2] = i1;
      }
      if (opt < cells[i1][i2].current_length) {
	opt = cells[i1][i2].current_length;
	opt_i1 = i1;
	opt_i2 = i2;
      }

    }
  }
  printf("%d\n", opt);
  if (opt >= 1) {
    int_pair_stack s;
    s.push(int_pair_stack::value_type(opt_i1, opt_i2));
    cell &temp1 = cells[s.top().first][s.top().second];
    assert(opt == temp1.current_length);
    while (temp1.current_length > 1) {
      cell &temp2 = cells[temp1.previous[0]][temp1.previous[1]];
      assert(temp2.current_length + 1 == temp1.current_length);
      s.push(int_pair_stack::value_type(temp1.previous[0], temp1.previous[1]));
      temp1 = cells[s.top().first][s.top().second];
    }
    while (!s.empty()) {
      // printf("s.top().first = %d, s.top().second = %d\n", s.top().first, s.top().second);
      assert(seq[0][s.top().first] == seq[1][s.top().second]);
      printf("%d ", seq[0][s.top().first]);
      s.pop();
    }
  }
  printf("\n");
  // fclose(stdin);
  // fclose(stdout);
  return 0;
}
