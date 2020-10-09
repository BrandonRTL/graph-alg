#ifndef __TGRAPH_H__
#define __TGRAPH_H__
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
class graph
{
public:
	std::vector<int> adj;
	std::vector<int> nums;
	std::vector<float> data;

	graph(std::string filename)
	{
		filename = "../../" + filename;
		std::ifstream file(filename);
		int num_row, num_col, num_lines;
		int sum_adj = 0;
		bool is_symetric = 0;
		std::string title;
		std::getline(file, title);
		is_symetric = title.find("symmetric");

		while (file.peek() == '%') file.ignore(2048, '\n');
		file >> num_row >> num_col >> num_lines;
		std::vector<std::vector<int>> temp_adj(num_row);
		std::vector<std::vector<float>> temp_data(num_row);

		for (int l = 0; l < num_lines; l++)
		{
			double data;
			int row, col;
			file >> row >> col >> data;
			temp_adj[row - 1].push_back(col - 1);
			temp_data[row - 1].push_back(data);
			if (is_symetric)
			{
				temp_adj[col - 1].push_back(row - 1);
				temp_data[col - 1].push_back(data);
			}

		}

		file.close();
		nums.push_back(0);
		for (int i = 0; i < num_row; i++)
		{
			for (int j = 0; j < temp_adj[i].size(); j++)
			{
				adj.push_back(temp_adj[i][j]);
				data.push_back(temp_data[i][j]);
				sum_adj++;
			}
			nums.push_back(sum_adj);
		}
//		for (int i = 0; i < adj.size(); i++)
//			std::cout << adj[i] << " ";
	}
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
void boruvkas_mst(graph gr);
int find_source_by_adj_number(graph& gra, int i);
#endif