// kruskal.cpp : Defines the entry point for the console application.
//

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#include <cstdio>

// TODO: reference additional headers your program requires here
#include <utility>
typedef std::pair<unsigned int, unsigned int> uint_pair;

#include <map>
typedef std::multimap<unsigned int, uint_pair> weight_edge_map;

#include <list>
typedef std::list<unsigned int> uint_list;

typedef struct vertex_vector_elem {
  uint_list *component_list;
} vertex_vector_elem;

#include <vector>
typedef std::vector<vertex_vector_elem> vertex_vector;

FILE *fin, *fout;

weight_edge_map minimum_spanning_forest (unsigned int vertex_count, weight_edge_map *edges) {
  vertex_vector vertices(vertex_count);
  for (unsigned int i1 = 0; i1 < vertices.size(); ++i1) {
    vertices[i1].component_list = new uint_list();
    vertices[i1].component_list->push_back(i1);
  }
  weight_edge_map forest_edges;
  for (weight_edge_map::iterator i2 = edges->begin(); i2 != edges->end(); ++i2) {
    uint_list *c1 = vertices[i2->second.first].component_list, *c2 = vertices[i2->second.second].component_list;
    if (c1 != c2) {
      // edge joins two vertices in different components.
      if (c1->size() > c2->size()) {
	for (uint_list::iterator i3 = c2->begin(); i3 != c2->end(); ++i3) {
	  c1->push_back(*i3);
	  vertices[*i3].component_list = c1;
	}
	delete(c2);
      } else {
	for (uint_list::iterator i3 = c1->begin(); i3 != c1->end(); ++i3) {
	  c2->push_back(*i3);
	  vertices[*i3].component_list = c2;
	}
	delete(c1);
      }
      forest_edges.insert(*i2);
    } else {
      //what, you want me to introduce a cycle in my forest?
    }
  }
  return forest_edges;
}

int main()
{
  fin = stdin;
  fout = stdout;
  unsigned int N;
  while (fscanf(fin, "%u", &N) != EOF) {
    weight_edge_map edges;
    for (unsigned int i1 = 0; i1 < N; ++i1) {
      for (unsigned int i2 = 0; i2 < N; ++i2) {
	unsigned int weight;
	fscanf(fin, "%u", &weight);
	edges.insert(weight_edge_map::value_type(weight, weight_edge_map::mapped_type(i1, i2)));
      }
    }
    weight_edge_map forest_edges = minimum_spanning_forest (N, &edges);
    unsigned int cost = 0;
    for (weight_edge_map::iterator i1 = forest_edges.begin(); i1 != forest_edges.end(); ++i1) {
      cost += i1->first;
    }
    fprintf(fout, "%u\n", cost);
  }
  fclose(fout);
  fclose(fin);
  return 0;
}
