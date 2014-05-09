#include <vector>
typedef std::vector<unsigned int> VU;
typedef std::vector<VU> VVU;
#include <string>
#include <cassert>
#define ASSERTS
#undef ASSERTS
#include <stack>
#include <iostream>

struct node {
  unsigned int begin;
  unsigned int end;
  std::vector<node *> children;
  node *suffix_link;
  bool belongs_in_s1;
  bool belongs_in_s2;
  node (): belongs_in_s1(false), belongs_in_s2(false), suffix_link(NULL) {}
};

struct suffix_tree {
  std::string s;
  node *nodes;
  node *root;
  // there will definitely be between n and 2 * n nodes, so we might
  // as well reserve that many.
  suffix_tree (std::string &s): s(s) {
    unsigned int n(this->s.size());
    this->nodes = new node[2 * n];
    node *nodes_front(nodes);
    //defining the root;
    this->root = nodes_front; ++nodes_front;
    this->root->begin = 0; this->root->end = 0;

    // initialising the active point to the root, when nothing remains
    // to be inserted.
    node *active_node(this->root);
    unsigned int active_length(0);
    char active_edge('\0');

    // again, nothing remains to be inserted.
    unsigned int remainder(0);
    for (unsigned int i1(1); i1 <= n; ++i1) {
      ++remainder;
      bool substring_exists(false);
      node *last_insertion_point(NULL);
      while (remainder > 0 && !substring_exists) {
	// make sure the active_length works well with the
	// active_node;
	bool active_length_ok(active_length <= 0);
	while (!active_length_ok) {
	  #ifdef ASSERTS
	  assert(active_edge != '\0');
	  #endif /* ASSERTS */
	  unsigned int i2(1);
	  for (;
	       i2 <= active_node->children.size() &&
		 this->s[active_node->children[i2 - 1]->begin] != active_edge;
	       ++i2) ;
	  #ifdef ASSERTS
	  assert(i2 <= active_node->children.size());
	  #endif /* ASSERTS */
	  if (active_length >=
	      active_node->children[i2 - 1]->end -
	      active_node->children[i2 - 1]->begin) {
	    active_length -=
	      active_node->children[i2 - 1]->end -
	      active_node->children[i2 - 1]->begin;
	    active_edge =
	      active_length <= 0 ?
	      '\0' : 
	      s[i1 - 1 - active_length]; 
	    active_node = active_node->children[i2 - 1];
	    active_length_ok = (active_edge == '\0');
	  }
	  else
	    active_length_ok = true;
	}
	// active_length is OK now.

	node *insertion_point(NULL);
	if (active_length <= 0) {
	  #ifdef ASSERTS
	  assert(active_edge == '\0');
	  #endif /* ASSERTS */
	  unsigned int i2(1);
	  for (;
	       i2 <= active_node->children.size() &&
		 this->s[active_node->children[i2 - 1]->begin] != this->s[i1 - 1 - active_length];
	       ++i2) ;
	  if (i2 > active_node->children.size()) 
	    insertion_point = active_node;
	  else {
            substring_exists = true;
            active_edge = s[active_node->children[i2 - 1]->begin];
            active_length = 1;
          }
	} else {
	  #ifdef ASSERTS
	  assert(active_edge != '\0');
	  #endif /* ASSERTS */
	  unsigned int i2(1);
	  for (;
	       i2 <= active_node->children.size() &&
		 this->s[active_node->children[i2 - 1]->begin] != this->s[i1 - 1 - active_length];
	       ++i2) ;
	  #ifdef ASSERTS
	  assert(i2 <= active_node->children.size());
	  #endif /*ASSERTS*/
	  if (this->s[active_node->children[i2 - 1]->begin + active_length] != this->s[i1 - 1]) {
	    nodes_front->begin = active_node->children[i2 - 1]->begin;
	    nodes_front->end = active_node->children[i2 - 1]->begin + active_length;
	    nodes_front->children.push_back(active_node->children[i2 - 1]);
	    active_node->children[i2 - 1]->begin += active_length;
	    active_node->children[i2 - 1] = nodes_front;

	    insertion_point = nodes_front; ++nodes_front;
	  } else {
            substring_exists = true;
            ++active_length;
          }
	}

	if (insertion_point != NULL) {
	  insertion_point->children.push_back(nodes_front);
	  nodes_front->begin = i1 - 1;
	  nodes_front->end = n;
	  ++nodes_front;

	  --remainder;

	  if (active_node != this->root)
	    active_node = (active_node->suffix_link != NULL) ? active_node->suffix_link : this->root;

	  if (active_node == this->root) {
	    if (remainder > 1) {
	      active_edge = s[i1 - remainder];
	      active_length = remainder - 1;
	    } else {
	      active_edge = '\0';
	      active_length = 0;
	    }
	  }
	  
	  if (last_insertion_point != NULL)
	    last_insertion_point->suffix_link = insertion_point;
	  last_insertion_point = insertion_point;	  
	}


      }
      
    }
  };
  ~suffix_tree () {delete nodes;}
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
    // for (int i1 = 0; i1 < 16; ++i1) {
    //   s1 += s1;
    //   s2 += s2;
    // }
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
	  for (unsigned int i2 = 0; !(current.second->belongs_in_s1) && i2 < current.second->children.size(); ++i2) {
	    if (current.second->children[i2]->belongs_in_s1)
	      current.second->belongs_in_s1 = true;
	  }
	  for (unsigned int i2 = 0; !(current.second->belongs_in_s2) && i2 < current.second->children.size(); ++i2) {
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
    std::cout << max_depth << std::endl;
  }

  // fclose(stdin);
  // fclose(stdout);
  return 0;
}
