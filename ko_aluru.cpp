#include <iostream>
#include <string>
#include <vector>
typedef std::vector<unsigned int> VU;
typedef std::vector<bool> VB;
#include <cassert>

struct SA{
  VU vu;
  VU suffix_array;
  SA (VU &vu): vu(vu) {
    unsigned int n(this->vu.size());
    VB is_s_type(n, false), is_l_type(n, false);
    unsigned int s_count(0), l_count(0);
    for (unsigned int i1 = 1, i2 = 2; i1 <= n;) {
      for (; i2 <= n && this->vu[i2 - 1] == this->vu[i1 - 1]; ++i2) ;
      for (; i1 < i2; ++i1) {
	if (i2 > n || this->vu[i1 - 1] < this->vu[i2 - 1]) {
	  is_s_type[i1 - 1] = true;
	  ++s_count;
	}
	if (i2 > n || this->vu[i2 - 1] < this->vu[i1 - 1]) {
	  is_l_type[i1 - 1] = true;
	  ++l_count;
	}
      }
    }
    //at the end of this loop, we've identified s-type and l-type
    //positions.
    assert(s_count + l_count == 1 + n);
    VU s_distance(n); {
      unsigned int i1;
      for (i1 = 1; (i1 < 2) || !is_s_type[i1 - 2]; ++i1)
	s_distance[i1 - 1] = 0;
      unsigned int i2(i1 - 1);;
      for (; i1 <= n; ++i1) {
	s_distance[i1 - 1] = i1 - i2;
	if (is_s_type[i1 - 1])
	  i2 = i1;
      }
    }
    // here, we have computed s-distances.
    
  }
  SA (std::string &s) {
    this->vu = VU(s.size() + 1);
    for (unsigned int i1 = 1; i1 <= vu.size(); ++i1) 
      this->vu[i1 - 1] = i1 <= s.size() ? ((unsigned int)s[i1 - 1]) : 0;
    SA sa(this->vu);
    this->suffix_array = sa.suffix_array;
  }

};

int main() {
  std::string s("mississippi");
  return 0;
}
