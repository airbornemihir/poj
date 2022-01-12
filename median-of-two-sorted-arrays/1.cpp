#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
  // Look at range a-b in nums1 and c-d in nums2. If nums1[(a+b)/2] >
  // nums2[(c+d)/2], then we're able to rule out the ranges ((a+b)/2)-b
  // in nums1 and c-((c+d)/2) as potentially containing the
  // median. Since these ranges add up to half the sizes of the ranges
  // we are initially looking at, we have reduced the problem size to half.
  double findMedianSortedArraysRanges
  (vector<int>& nums1, vector<int>& nums2, unsigned int a1, unsigned int a2, unsigned int p) {
    if (a2 >= nums2.size())
      {return nums1[a1 + p];}
    else if (a1 >= nums1.size())
      {return nums2[a2 + p];}
    else if ((p <= 0) && (nums1[a1] < nums2[a2]))
      {return nums1[a1];}
    else if (p <= 0)
      {return nums2[a2];}
    else if ((a2 + p - (p/2) >= nums2.size()) || ((a1 + p - (p/2) >= nums1.size()) && (nums1[a1 + p - (p/2)] < nums2[a2 + p - (p/2)])))
      {return findMedianSortedArraysRanges
          (nums1, nums2, a1 + p - (p/2), a2, p/2);}
    else
      {return findMedianSortedArraysRanges
          (nums1, nums2, a1, a2 + p - (p/2), p/2);}
  }

  double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    return
      findMedianSortedArraysRanges
      (nums1, nums2, 0, 0, (nums1.size() + nums2.size() - 1)/2);
  }
};

int main () {
  Solution s;
  vector<int> nums1 {1, 3};
  vector<int> nums2 {2};
  std::cout << s.findMedianSortedArrays(nums1, nums2) << std::endl;
  return 0;}
