/*********************************************************************
 * Authors: Malte Helmert (helmert@informatik.uni-freiburg.de),
 *          Silvia Richter (silvia.richter@nicta.com.au)
 * (C) Copyright 2003-2004 Malte Helmert and Silvia Richter
 *
 * This file is part of LAMA.
 *
 * LAMA is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the license, or (at your option) any later version.
 *
 * LAMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *********************************************************************/

#include "scc.h"
#include <algorithm>
#include <vector>
using namespace std;

vector<vector<int> > SCC::get_result() {
  // Get node count
  int node_count = graph.size();
  // initialise vector<int>, first three indexed by vertex number
  dfs_numbers.resize(node_count, -1);
  dfs_minima.resize(node_count, -1);
  stack_indices.resize(node_count, -1);
  // vector<int> This is indexed by the level of recursion.
  stack.reserve(node_count);
  current_dfs_number = 0;

  // For each node
  for(int i = 0; i < node_count; i++)
    if(dfs_numbers[i] == -1)
      dfs(i);

  reverse(sccs.begin(), sccs.end());
  return sccs;
}

void SCC::dfs(int vertex) {
  // current_dfs_number: global variable that indicates current depth
  int vertex_dfs_number = current_dfs_number++;
  dfs_numbers[vertex] = dfs_minima[vertex] = vertex_dfs_number;
  // stack is an stack of vertex that leads to this vertex
  stack_indices[vertex] = stack.size();
  stack.push_back(vertex);

  // For each successor of the vertex
  const vector<int> &successors = graph[vertex];
  for(int i = 0; i < successors.size(); i++) {
	 // get index and dfs_number of the successor
    int succ = successors[i];
    int succ_dfs_number = dfs_numbers[succ];
    // If the succ has not been analyzed yet
    if(succ_dfs_number == -1) {
      // Analyze
      dfs(succ);
      // The minimum dfs is the min value between the current min and the dfs of the successor
      dfs_minima[vertex] = min(dfs_minima[vertex], dfs_minima[succ]);
    }
    // If the succ has been analyzed, the depth of the successor is minor than current and
    // the successor can be found in the satack of the current vertex
    else if(succ_dfs_number < vertex_dfs_number && stack_indices[succ] != -1) {
      // Set the minimum dfs
      dfs_minima[vertex] = min(dfs_minima[vertex], succ_dfs_number);
    }
  }

  // If the minimum dfs for the current vertex is the current one after analyzing all successors
  if(dfs_minima[vertex] == vertex_dfs_number) {
	 // Get stack index
    int stack_index = stack_indices[vertex];
     //Create scc: a stack of strongly connected components following the minimum stack
    vector<int> scc;
    for(int i = stack_index; i < stack.size(); i++) {
      // Add the component
      scc.push_back(stack[i]);
      // Remove the index and set to default
      stack_indices[stack[i]] = -1;
    }
    // Erase the stack
    stack.erase(stack.begin() + stack_index, stack.end());
    // Add the scc to the list of sccs
    sccs.push_back(scc);
  }
}

/*
#include <iostream>
using namespace std;

int main() {

#if 0
  int n0[] = {1, -1};
  int n1[] = {-1};
  int n2[] = {3, 4, -1};
  int n3[] = {2, 4, -1};
  int n4[] = {0, 3, -1};
  int *all_nodes[] = {n0, n1, n2, n3, n4, 0};
#endif

  int n0[] = {-1};
  int n1[] = {3, -1};
  int n2[] = {4, -1};
  int n3[] = {8, -1};
  int n4[] = {0, 7, -1};
  int n5[] = {4, -1};
  int n6[] = {5, 8, -1};
  int n7[] = {2, 6, -1};
  int n8[] = {1, -1};
  int *all_nodes[] = {n0, n1, n2, n3, n4, n5, n6, n7, n8, 0};

  vector<vector<int> > graph;
  for(int i = 0; all_nodes[i] != 0; i++) {
    vector<int> successors;
    for(int j = 0; all_nodes[i][j] != -1; j++)
      successors.push_back(all_nodes[i][j]);
    graph.push_back(successors);
  }

  vector<vector<int> > sccs = SCC(graph).get_result();
  for(int i = 0; i < sccs.size(); i++) {
    for(int j = 0; j < sccs[i].size(); j++)
      cout << " " << sccs[i][j];
    cout << endl;
  }
}
*/
