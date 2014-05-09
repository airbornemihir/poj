#include <vector>
typedef std::vector<unsigned int> VU;
typedef std::vector<VU> VVU;
#include <string>
#include <cassert>
#define ASSERTS

struct node {
  unsigned int begin;
  unsigned int end;
  std::vector<node *> children;
  node *suffix_link;
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
            active_edge = s[active_node->children[i2]->begin];
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
	    insertion_point = active_node->children[i2 - 1];

	    nodes_front->begin = active_node->children[i2 - 1]->begin;
	    nodes_front->end = active_node->children[i2 - 1]->begin + active_length;
	    nodes_front->children.push_back(active_node->children[i2 - 1]);
	    active_node->children[i2 - 1]->begin += active_length;
	    active_node->children[i2 - 1] = nodes_front;

	    ++nodes_front;
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
	    active_node = (active_node->suffix_link != NULL) ? active_node->suffix_link : nodes;

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

int main() {
  unsigned int n(1000);
  int *array = new int(2 * n);

  return 0;
}
