#include <vector>
#include <map>

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
  ListNode* mergeKLists(vector<ListNode*>& lists) {
    multimap<int, ListNode*> firsts;
    for (int i1 = 0; i1 < lists.size(); ++i1) {
      if (lists[i1] != nullptr) {
        firsts.insert({lists[i1]->val, lists[i1]->next});
      }
    }
    ListNode *head = nullptr;
    ListNode **r= &head;
    for (multimap<int, ListNode*>::iterator i2 = firsts.begin();
         i2 != firsts.end();
         i2 = firsts.begin()) {
      int val = i2->first; ListNode *next = i2->second;
      *r = new ListNode (val);
      r = &((**r).next);
      firsts.erase(i2);
      if (next != nullptr) {
        firsts.insert({next->val, next->next});
      }
    }
    return head;
  }
};

int main ()
{return 0;}
