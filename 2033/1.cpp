#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

typedef std::vector<int> int_vector;

void compute_element(int_vector &A, std::string &s, int first_not_used) {
  if (A[first_not_used] < 0) {
    A[first_not_used] = 0;
    if ((first_not_used - 1 >= 0) &&
        ('1' <= s[first_not_used - 1]) && (s[first_not_used - 1] <= '9')) {
      compute_element(A, s, first_not_used - 1);
      A[first_not_used] += A[first_not_used - 1];
    }
    if ((first_not_used - 2 >= 0) &&
        ((('0' < s[first_not_used - 2]) && (s[first_not_used - 2] < '2')) ||
         ((s[first_not_used - 2] == '2') &&
          ('0' <= s[first_not_used - 1]) && (s[first_not_used - 1] <= '6')))) {
      compute_element(A, s, first_not_used - 2);
      A[first_not_used] += A[first_not_used - 2];
    }
  }
}

int main() {
  std::string s;
  std::cin >> s;
  while (s != "0") {
    int N = s.size();
    int_vector A(N + 1, -1);
    A[0] = 1;
    compute_element(A, s, N);
    std::cout << A[N] << std::endl;

    std::cin >> s;
  }

  return 0;
}
