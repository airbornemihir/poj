#include <cstdio>
#include <vector>
typedef std::vector<int> int_vector;

FILE *fin, *fout;

int main() {
  fin = stdin;
  fout = stdout;
  int size = 1;
  int_vector f1(size, 1), f2(size, 0);
  while (size <= 15) {
    f1.push_back(3*f1[size - 1] + 2*f2[size - 1]);
    f2.push_back(f1[size - 1] + f2[size - 1]);
    ++size;
  }
  
  int n;
  fscanf(fin, "%d", &n);
  while (n >= 0) {
    int result = 0;
    if (n % 2 == 0) {
      result = f1[n / 2];
    }
    fprintf(fout, "%d\n", result);
    fscanf(fin, "%d", &n);
  }

  fclose(fout);
  fclose(fin);
  return 0;
}
