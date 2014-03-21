#include <cstdio>
#include <vector>
#include <string>
#include <cassert>
#include <queue>
#include <stack>
#include <utility>
#include <iostream>

struct node {
  unsigned int begin;
  unsigned int end;
  std::vector<node *> children;
  node *parent;
  node *suffix_link;
  bool belongs_in_s1;
  bool belongs_in_s2;
  node(node *parent, unsigned int begin, unsigned int end):
    parent(parent),
    begin(begin),
    end(end),
    suffix_link(NULL),
    belongs_in_s1(false),
    belongs_in_s2(false)
  {}
};

struct suffix_tree {
  std::string s;
  node *root;
  suffix_tree(std::string &s): s(s) {
    unsigned int n = s.size();
    this->root = new node(NULL, 0, 0);
    unsigned int remainder = 0;
    node *active_node = this->root;
    char active_edge = '\0';
    unsigned int active_length = 0;
    for (unsigned int i1 = 1; i1 <= n; ++i1) {
      ++remainder;

      bool substring_already_exists = false;
      node *last_insertion_point = NULL;
      while ((remainder > 0) && (!substring_already_exists)) {
        //our first job is to make sure the active_node and
        //active_edge support the active_length.
        bool active_node_ok = (active_edge == '\0');
        while (!active_node_ok) {
          //guaranteed to have a non-null active_edge.
          assert(active_length > 0);
          unsigned int i2;
          for (i2 = 0;
               (i2 < active_node->children.size()) &&
                 (s[active_node->children[i2]->begin] != active_edge);
               ++i2);
          assert(i2 < active_node->children.size());
          if (active_node->children[i2]->begin + active_length <
              active_node->children[i2]->end) 
            active_node_ok = true;
          else {
            active_length -=
	      active_node->children[i2]->end -
	      active_node->children[i2]->begin;
            active_node = active_node->children[i2];
            active_edge =
	      (active_length <= 0) ?
              '\0' :
              s[i1 - 1 - active_length];
            active_node_ok = (active_edge == '\0');
          }
        }
        //here, we have a guarantee that the active_edge contains more
        //characters than the active_length.
        //now, we need to separate the cases for insertion and
        //non-insertion.
        //insertion occurs either if active_edge is '\0' and we don't
        //find a matching edge for the character s[i1 - 1], or else if
        //the active_edge is something else but still doesn't contain
        //'\0';
        node *insertion_point = NULL;
        if (active_edge == '\0') {
          assert(active_length <= 0);
          unsigned int i2;
          for (i2 = 0;
               (i2 < active_node->children.size()) && (s[active_node->children[i2]->begin] != s[i1 - 1]);
               ++i2);
          if (i2 < active_node->children.size()) {
            substring_already_exists = true;
            active_edge = s[active_node->children[i2]->begin];
            active_length = 1;
          } else {
            insertion_point = active_node;
          }
        } else {
          assert(active_length > 0);
          unsigned int i2;
          for (i2 = 0;
               (i2 < active_node->children.size()) && (s[active_node->children[i2]->begin] != s[i1 - 1 - active_length]);
               ++i2);
          assert(i2 < active_node->children.size());
          if (s[active_node->children[i2]->begin + active_length] == s[i1 - 1]) {
            substring_already_exists = true;
            ++active_length;
          } else {
            node *temp1 = new node(active_node,
				   active_node->children[i2]->begin,
                                   active_node->children[i2]->begin + active_length);
            temp1->children.push_back(active_node->children[i2]);
	    active_node->children[i2]->parent = temp1;
	    active_node->children[i2]->begin += active_length;
            active_node->children[i2] = temp1;
            insertion_point = temp1;
          }
        }
        if (insertion_point != NULL) {
          node *temp1 = new node(insertion_point, i1 - 1, n);
          insertion_point->children.push_back(temp1);
          --remainder;
          if (active_node == this->root) {
            if (remainder > 1) {
              active_edge = s[i1 - remainder];
              --active_length;
            } else {
              active_edge = '\0';
              active_length = 0;
            }
          } else {
            if (active_node->suffix_link != NULL)
              active_node = active_node->suffix_link;
            else {
              active_node = this->root;
	      // new stuff!
	      if (remainder > 1) {
		active_edge = s[i1 - remainder];
		active_length = remainder - 1; // what if remainder ==
					       // 0 ?
	      } else {
		active_edge = '\0';
		active_length = 0;
	      }
            }
          }
          if (last_insertion_point != NULL) 
            last_insertion_point->suffix_link = insertion_point;
          last_insertion_point = insertion_point;
        }
      }
    }
  }
};

typedef std::pair<bool, node *> marked_node;
typedef std::stack<marked_node> marked_node_stack;
int main() {
  // freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);

  {
    std::string s1, s2;
    std::cin >> s1;
    std::cin >> s2;
    std::string s = s1 + std::string("$") + s2 + std::string("#");
    suffix_tree t(s);
    unsigned int n1 = s1.size();
    unsigned int n2 = s2.size();
    marked_node_stack dfs_stack;
    unsigned int depth = 0;
    dfs_stack.push(marked_node(false, t.root));
    unsigned int max_depth = 0;
    while (!dfs_stack.empty()) {
      marked_node current = dfs_stack.top();
      dfs_stack.pop();
      if (current.first) {
	if (current.second->children.size() > 0) {
	  for (unsigned int i2 = 0; i2 < current.second->children.size(); ++i2) {
	    if (current.second->children[i2]->belongs_in_s1)
	      current.second->belongs_in_s1 = true;
	    if (current.second->children[i2]->belongs_in_s2)
	      current.second->belongs_in_s2 = true;
	  }	  
	  max_depth =
	    (current.second->belongs_in_s1 &&
	     current.second->belongs_in_s2 &&
	     (max_depth < depth)) ?
	    depth :
	    max_depth;
	} else {
	  if (current.second->begin <= n1) 
	    current.second->belongs_in_s1 = true;
	  else
	    current.second->belongs_in_s2 = true;
	  // for (node *temp1 = current.second->parent;
	  //      temp1 != NULL;
	  //      temp1 = temp1->parent)
	  //   if (current.second->begin <= n1) 
	  //     temp1->belongs_in_s1 = true;
	  //   else
	  //     temp1->belongs_in_s2 = true;
	}

	depth -= current.second->end - current.second->begin;
      } else {
	current.first = true;
	dfs_stack.push(current);
	depth += current.second->end - current.second->begin;

	for (unsigned int i2 = 0; i2 < current.second->children.size(); ++i2) 
	  dfs_stack.push(marked_node(false, current.second->children[i2]));

      }
    }
    printf("%u\n", max_depth);
  }

  // fclose(stdin);
  // fclose(stdout);
  return 0;
}
