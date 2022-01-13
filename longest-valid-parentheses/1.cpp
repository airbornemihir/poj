#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution {
public:
  int longestValidParenthesesLookup(string s, std::vector<std::vector<int> > &v, int m, int n) {
    if (v[m][n] < 0) {
      if (n - m <= 1) {
        v[m][n] = 0;
      }
      else if (s[m] == ')') /*We're starting with an rparen...*/ {
        v[m][n] = longestValidParenthesesLookup(s, v, m + 1, n);
      }
      else if (s[n - 1] == '(') /*We're ending with an lparen...*/ {
        v[m][n] = longestValidParenthesesLookup(s, v, m, n - 1);
      }
      /* We're starting and ending properly and containing a full string.*/
      else if (longestValidParenthesesLookup(s, v, m + 1, n - 1) == n - m - 2) {
        v[m][n] = n - m;
      } else {
        // This is the default assumption, that the lparen at the
        // start is not closed and so we have to take what we can.
        v[m][n] = longestValidParenthesesLookup(s, v, m + 1, n);
        for (int i1 = m + 1; i1 < n; i1++) {
          if ((longestValidParenthesesLookup(s, v, m, i1) == i1 - m) &&
              (longestValidParenthesesLookup(s, v, i1, n) == n - i1)) {
            v[m][n] = n - m; break;
          }
          else {
            if (v[m][n] <= longestValidParenthesesLookup(s, v, m, i1)) {
              v[m][n] = longestValidParenthesesLookup(s, v, m, i1);
            }
            if (v[m][n] <= longestValidParenthesesLookup(s, v, i1, n)) {
              v[m][n] = longestValidParenthesesLookup(s, v, i1, n);
            }
          }
        }
      }
    }
    return v[m][n];
  }

  int longestValidParentheses(string s) {
    std::vector<std::vector<int> > v(s.length() + 1, std::vector<int>(s.length() + 1, -1));
    return longestValidParenthesesLookup(s, v, 0, s.length());
  }
};

int main () {
  Solution s;
  std::cout << s.longestValidParentheses("(()()(()(()))()((()))((()(()())()(()))())))()(()()))())))))))()()()()))(((()())((()()(((())))()(()()(())((()))))))(()(()))(((()())()))(()))((((()(()()()())()()(()))(()()(())()((()()())))(())()())()(") << std::endl;
  return 0;
}
