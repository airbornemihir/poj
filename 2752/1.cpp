#include <vector>
typedef std::vector<unsigned int> VU;
typedef std::vector<VU> VVU;
#include <string>
#include <cassert>
#define ASSERTS
#undef ASSERTS
#define ASSERTS
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
  ~suffix_tree () {delete[] nodes;}
};

void get_max_depth(node *current,
                   unsigned int *depth,
                   unsigned int *max_depth,
                   unsigned int n1,
                   unsigned int n2) {
  *depth += current->end - current->begin;

  for (unsigned int i2 = 1; i2 <= current->children.size(); ++i2) 
    get_max_depth(current->children[i2 - 1], depth, max_depth, n1, n2);

  // non-leaf, has children in s1 or s2, hopefully both.
  if (current->children.size() > 0) {
    for (unsigned int i2 = 0; !(current->belongs_in_s1) && i2 < current->children.size(); ++i2) 
      if (current->children[i2]->belongs_in_s1) current->belongs_in_s1 = true;

    for (unsigned int i2 = 0; !(current->belongs_in_s2) && i2 < current->children.size(); ++i2) 
      if (current->children[i2]->belongs_in_s2) current->belongs_in_s2 = true;

    if (current->belongs_in_s1 && current->belongs_in_s2 && (*max_depth < *depth))
      *max_depth = *depth;
  }
  // leaf in s1.
  else if (current->begin <= n1) current->belongs_in_s1 = true;
  // leaf in s2.
  else current->belongs_in_s2 = true;

  *depth -= current->end - current->begin;
}

int main() {
  std::string S;
  for (std::cin >> S; !std::cin.eof(); std::cin >> S) {
    S += "$";
    unsigned int n(S.size());
    suffix_tree T(S);
    unsigned int position(0); node *current(T.root);
    unsigned int substring_count(0);
    while (position < n) {
      node *next(NULL);
      for (unsigned int i2 = 1; i2 <= current->children.size(); ++i2) {
	if (current != T.root && current->children[i2 - 1]->begin == n - 1)
	  std::cout << current->end << " ";
	if (S[current->children[i2 - 1]->begin] == S[position]) {
	  #ifdef ASSERTS
	  assert(next == NULL);
	  #endif /* ASSERTS */
	  next = current->children[i2 - 1];
	}
      }
      #ifdef ASSERTS
      assert(next != NULL);
      #endif /* ASSERTS */
      position = next->end;
      current = next;
      
    }
    std::cout << (n - 1) << std::endl;
  }

  return 0;
}
