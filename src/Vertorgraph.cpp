#include "Vectorgraph.h"
int find(std::vector<subtree>& t_tree, int i)
{
    int root = i;
    while (t_tree[root].parent != root)
    {
        root = t_tree[root].parent;
    }

    while (t_tree[i].parent != root)
    {
        int tmp = t_tree[i].parent;
        t_tree[i].parent = root;
        i = root;
    }

    return root;
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
void boruvkas_mst(graph& gr)
{
    subtree t_tree;
    int V = gr.nums.size() - 1;
    int E = gr.data.size();
    std::vector<subtree> trees(V);
    int tree_counter = V;
    int tree_control = 0;
    double tree_weight = 0;
    std::cout << "MST" << std::endl;

    for (int i = 0; i < V; i++)
    {
        t_tree = subtree(i, 0);
        trees[i] = t_tree;
    }
    int start = clock();
    while (tree_counter > 1)
    {
        std::vector<int> smallest_edge(V, -1);
        int tree_control = tree_counter;
        for (int i = 0; i < V; i++)
        {
            int set_b = find(trees, i);
            for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
            {
                int set_a = find(trees, gr.adj[j]);
                if (set_a != set_b)
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
                int set_a = find(trees, gr.adj[smallest_edge[i]]);
                int set_b = find(trees, gr.sec_ver[smallest_edge[i]]);

                if (set_a != set_b)
                {
                    tree_weight += gr.data[smallest_edge[i]];
                    //	std::cout << "Edge (" << find_source_by_adj_number(gr, smallest_edge[i]) << ","
                    //		<< gr.adj[smallest_edge[i]] << ") " << "weight "
                    //		<< gr.data[smallest_edge[i]] << std::endl;
                    tree_union(trees, set_a, set_b);
                    tree_control--;
                }
            }
        }
        if (tree_control == tree_counter)
            break;
        tree_counter = tree_control;
    }
    std::cout << (clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Total weight: " << tree_weight << std::endl;
}
