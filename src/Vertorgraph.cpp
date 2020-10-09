#include "Vectorgraph.h"
int find(std::vector<subtree>& t_tree, int i)
{
	if (t_tree[i].parent != i)
		t_tree[i].parent = find(t_tree, t_tree[i].parent);

	return t_tree[i].parent;
}
void tree_union(std::vector<subtree>& t_tree, int a, int b)
{
	int root_a = find(t_tree, a);
	int root_b = find(t_tree, b);
	if (t_tree[root_a].rank < t_tree[root_b].rank)
		t_tree[root_a].parent = root_b;
	else if (t_tree[root_a].rank > t_tree[root_b].rank)
		t_tree[root_b].parent = root_a;
	else
	{
		t_tree[root_b].parent = root_a;
		t_tree[root_a].rank = t_tree[root_a].rank + 1;
	}
}
int find_source_by_adj_number(graph& gra, int i)
{
	int j = 1;
	for (int k = 0; k < i; k++)
	{
		if (gra.nums[j] <= i)
			j++;
	}
	return j - 1;
}
std::vector<mst_node> boruvkas_mst(graph gr)
{
	std::vector<subtree> trees; 
	subtree t_tree;
	int V = gr.nums.size() - 1;
	int E = gr.data.size();
	int tree_counter = V;
	float tree_weight = 0;
	std::vector<mst_node> mst;
	std::cout << "MST" << std::endl;
	
	for (int i = 0; i < V; i++)
	{
		t_tree = subtree(i, 0);
		trees.push_back(t_tree);
	}
	for (int i = 0; i < V - 1; i++)
	{
		mst_node temp(0, 0);
		mst.push_back(temp);
	}
	
	while (tree_counter > 1)
	{
		std::vector<int> smallest_edge(V, -1);
		for (int i = 0; i < V; i++)
		{
			for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
			{

				int set_a = find(trees, gr.adj[j]);
				int set_b = find(trees, i);
				
				if (set_a == set_b)
					continue;
				else
				{
					if (smallest_edge[set_a] == -1 || gr.data[smallest_edge[set_a]] > gr.data[j])
						smallest_edge[set_a] = j;
					if (smallest_edge[set_b] == -1 || gr.data[smallest_edge[set_b]] > gr.data[j])
						smallest_edge[set_b] = j;
				}
			}
		}
		for (int i = 0; i<V; i++)
		{
			if (smallest_edge[i] != -1)
			{
				int j = 0;
				int set_a = find(trees, gr.adj[smallest_edge[i]]);
				int set_b = find(trees, find_source_by_adj_number(gr, smallest_edge[i]));

				if (set_a == set_b)
					continue;
			//	mst_node temp(find_source_by_adj_number(gr, smallest_edge[i]), gr.data[smallest_edge[i]]);
				mst_node temp(gr.adj[smallest_edge[i]], gr.data[smallest_edge[i]]);
				tree_weight += gr.data[smallest_edge[i]];
				mst[find_source_by_adj_number(gr, smallest_edge[i])] = temp;
				/*
				std::cout << "Edge (" << find_source_by_adj_number(gr, smallest_edge[i]) << ","
					<< gr.adj[smallest_edge[i]] << ") " << "weight "
					<< gr.data[smallest_edge[i]] << std::endl;
					*/
				tree_union(trees, set_a, set_b);
				tree_counter--;
				
			}
		}
	}
	/*
	for (int i = 0; i < V - 1; i++)
	{
		std::cout << i << " " << mst[i].parent << " " << mst[i].weight << std::endl;
	}
	std::cout << "Total weight: " << tree_weight << std::endl;
	*/
	return mst;
}
