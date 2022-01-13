#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
      // The k part is not strictly necessary.
      vector<ListNode*> pointers(k);
      int i1 = 0; ListNode *cur = head;
      while ((i1 < k) && (cur != nullptr)) {
        pointers[i1] = cur->next;
        cur = cur->next;
        ++i1;
      }
      if ((k <= 1) || (i1 < k)) {
        return head;
      }
      else {
        for (int i2 = 0; i2 < k - 1; i2++) {
          pointers[i2]->next = (i2 == 0) ? head : pointers[i2 - 1];
        }
        head->next = reverseKGroup(pointers[k - 1], k);
        return pointers[k - 2];
      }
    }
};

int main () {
return 0;}
