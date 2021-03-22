#ifndef __TGRAPH_H__
#define __TGRAPH_H__
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
class graph
{
public:
    std::vector<int> adj;
    std::vector<int> nums;
    std::vector<int> sec_ver;
    std::vector<float> data;
    graph(std::string filename);
};
class subtree
{
public:
    int parent;
    int rank;
    subtree(int pr = 0, int r = 0)
    {
        parent = pr;
        rank = r;
    }
};

int find(std::vector<subtree>& subsets, int i);
void tree_union(std::vector<subtree>& subsets, int a, int b);
void boruvkas_mst(graph& gr);
void boruvkas_parallel(graph& gr);
void print_graph(graph gr, std::string graphName);
void print_arr(std::vector<int> new_adjs_temp, int size);
void print_removed_smallest_edge(std::vector<int> smallest_edge, int size);
#endif