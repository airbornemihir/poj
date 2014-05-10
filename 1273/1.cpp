// drainage_ditches.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include <cassert>
#include <climits>

// TODO: reference additional headers your program requires here

typedef struct edge {
  unsigned int src;
  unsigned int dst;
  unsigned int cap;
  unsigned int index;
} edge;

#include <vector>
typedef std::vector<edge> edge_vector;
typedef std::vector<edge_vector> adjacency_vector;

#include <queue>
typedef std::queue<unsigned int> uint_queue;

typedef std::vector<unsigned int> uint_vector;
typedef std::vector<bool> bool_vector;

typedef std::vector<edge *> backtrack_vector;

#include <stack>
typedef std::pair<bool, unsigned int> marked_uint;
typedef std::stack<marked_uint> marked_uint_stack;

FILE *fin, *fout;

void dotestcase(unsigned int N, unsigned int M) {
  adjacency_vector out_adj(M);
  for (unsigned int i1 = 0; i1 < N; ++i1) {
    edge temp1, temp2;
    unsigned int src, dst, cap;
    fscanf(fin, "%u", &src);
    fscanf(fin, "%u", &dst);
    fscanf(fin, "%u", &cap);
    temp1.src = src;
    temp1.dst = dst;
    temp1.cap = cap;
    temp1.index = out_adj[dst - 1].size();
    out_adj[src - 1].push_back(temp1);
    temp2.src = dst;
    temp2.dst = src;
    temp2.cap = 0;
    temp2.index = out_adj[src - 1].size() - 1;
    out_adj[dst - 1].push_back(temp2);
  }
  //input done.
  unsigned int flow = 0; //we'll augment this!
  bool done = false;
  while (!done) {
    //prepare for bfs!
    uint_queue bfs_queue;
    bool_vector bfs_seen(M, false);
    uint_vector rank_vector(M);
    bfs_queue.push(1); // pushing the source
    bfs_seen[1 - 1] = true;
    rank_vector[1 - 1] = 0;
    //run the bfs!
    while (!bfs_queue.empty()) {
      unsigned int current = bfs_queue.front();
      bfs_queue.pop();
      for (edge_vector::iterator i1 = out_adj[current - 1].begin();
	   i1 != out_adj[current - 1].end();
	   ++i1) {
	assert(i1->src == current);
	if (!bfs_seen[i1->dst - 1] && (i1->cap > 0)) {
	  bfs_queue.push(i1->dst);
	  bfs_seen[i1->dst - 1] = true;
	  rank_vector[i1->dst - 1] = 1 + rank_vector[i1->src - 1];
	}
      }
    }
    //bfs done!
    if (!bfs_seen[M - 1]) { // sink is not seen.
      done = true;
    } else {
      marked_uint_stack dfs_stack;
      bool_vector dfs_seen(M, false);
      backtrack_vector dfs_backtracker(M, NULL);
      dfs_stack.push(marked_uint(false, 1)); //pushing the source
      dfs_seen[1 - 1] = true;
      dfs_backtracker[1 - 1] = NULL;
      while (!dfs_stack.empty()) {
	marked_uint current = dfs_stack.top();
	dfs_stack.pop();
	if (current.first) { //all children popped, so pop it.
	  //this may end in the sink, check now.
	  if (current.second == M) { //sink!
	    unsigned int flow_increment = UINT_MAX;
	    edge *first_critical_edge;
	    for (edge *i1 = dfs_backtracker[current.second - 1];
		 i1 != NULL;
		 i1 = dfs_backtracker[i1->src - 1]) {
	      if (i1->cap <= flow_increment) {
		flow_increment = i1->cap;
		first_critical_edge = i1;
	      }
	    }
	    flow += flow_increment;
	    for (edge *i1 = dfs_backtracker[current.second - 1];
		 i1 != NULL;
		 i1 = dfs_backtracker[i1->src - 1]) {
	      i1->cap -= flow_increment;
	      out_adj[i1->dst - 1][i1->index].cap += flow_increment;
	    }
	    marked_uint temp1 = current;
	    while (dfs_backtracker[temp1.second - 1] != first_critical_edge) {
	      assert(!dfs_stack.empty());
	      temp1 = dfs_stack.top();
	      dfs_stack.pop();
	    }
	  }
	} else {
	  current.first = true;
	  dfs_stack.push(current); //we're going to push children
				   //next.
	  for (edge_vector::iterator i1 = out_adj[current.second - 1].begin();
	       i1 != out_adj[current.second - 1].end();
	       ++i1) {
	    if ((i1->cap > 0) &&
		(rank_vector[i1->dst - 1] == rank_vector[i1->src - 1] + 1)) {
	      dfs_stack.push(marked_uint(false, i1->dst));
	      dfs_seen[i1->dst - 1] = true;
	      dfs_backtracker[i1->dst - 1] = &(*i1); // segmentation fault?
	    }
	  }
	}
      }
    }
  }
  fprintf(fout, "%u\n", flow);
}

int main()
{
  fin = stdin;
  fout = stdout;
  unsigned int N, M;
  while (fscanf(fin, "%u %u", &N, &M) != EOF) {
    dotestcase(N, M);
  }
  fclose(fout);
  fclose(fin);
  return 0;
}

