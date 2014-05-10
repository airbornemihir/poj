#include <cstdio>
#include <vector>

typedef std::vector<unsigned int> uint_vector;
typedef std::vector<uint_vector> uint_vector_vector;
typedef std::vector<bool> bool_vector;
typedef std::vector<bool_vector> bool_vector_vector;

FILE *fin, *fout;

int main() {
  fin = stdin;
  fout = stdout;
  unsigned int N;
  fscanf(fin, "%u", &N);
  uint_vector_vector triangle;
  bool_vector_vector seen;
  for (unsigned int i1 = 1; i1 <= N; ++i1) {
    uint_vector temp1(i1);
    bool_vector temp2(i1);
    for (unsigned int i2 = 1; i2 <= i1; ++i2) {
      fscanf(fin, "%u", &temp1[i2 - 1]);
      temp2[i2 - 1] = false;
    }
    triangle.push_back(temp1);
    seen.push_back(temp2);
  }
  for (unsigned int i1 = N - 1; i1 >= 1; --i1) {
    for (unsigned int i2 = 1; i2 <= i1; ++i2) {
      unsigned int temp1 = triangle[i1 + 1 - 1][i2 - 1], temp2 = triangle[i1 + 1 - 1][i2 + 1 - 1];
      triangle[i1 - 1][i2 - 1] += (temp2 > temp1 ? temp2 : temp1);
    }
  }
  fprintf(fout, "%u\n", triangle[1 - 1][1 - 1]);

  fclose(fout);
  fclose(fin);
  return 0;
}
