#include "Vectorgraph.h"
#include <iomanip>

//#define PRINT_CONTROL

using namespace std;
graph::graph(std::string filename)
{
    filename = "../../" + filename;
    std::ifstream file(filename);
    int num_row, num_col, num_lines;
    int sum_adj = 0;
    bool is_symetric = 0;
    std::string title;
    std::getline(file, title);
    if (title.find("symmetric") != std::string::npos)
    {
        is_symetric = 1;
    };
    //std::cout << title << std::endl;
    while (file.peek() == '%') file.ignore(2048, '\n');
    file >> num_row >> num_col >> num_lines;
    std::vector<std::vector<int>> temp_adj(num_row);
    std::vector<std::vector<float>> temp_data(num_row);
    nums.resize(num_row + 1);
    if (is_symetric)
    {
        sec_ver.resize(num_lines);
    }
    sec_ver.resize(2 * num_lines);
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
    nums[0] = 0;
    int counter = 0;
    for (int i = 0; i < num_row; i++)
    {
        for (int j = 0; j < temp_adj[i].size(); j++)
        {
            adj.push_back(temp_adj[i][j]);
            data.push_back(temp_data[i][j]);
            sum_adj++;
            sec_ver[counter] = i;
            counter++;
        }
        nums[i + 1] = sum_adj;
    }
}
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
        /*
        for (int i = 0; i < V; i++)
        {
        cout << i << "'s smallest edge: " << smallest_edge[i] << " (" << gr.adj[smallest_edge[i]] << "," << gr.sec_ver[smallest_edge[i]] << ") weight:" << gr.data[smallest_edge[i]] << endl;
        }
        */
        for (int i = 0; i<V; i++)
        {
            if (smallest_edge[i] != -1)
            {
                int set_a = find(trees, gr.adj[smallest_edge[i]]);
                int set_b = find(trees, gr.sec_ver[smallest_edge[i]]);

                if (set_a != set_b)
                {
                    tree_weight += gr.data[smallest_edge[i]];
                    //std::cout << "Edge (" << gr.sec_ver[smallest_edge[i]] << ","
                    //<< gr.adj[smallest_edge[i]] << ") " << "weight "
                    //	<< gr.data[smallest_edge[i]] << std::endl;
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
    std::cout << "Total weight: " << std::setprecision(10) << tree_weight << std::endl;
}
void boruvkas_parallel(graph& gr)
{
    subtree t_tree;
    int V = gr.nums.size() - 1;
    int E = gr.data.size();
    std::vector<subtree> trees(V);
    int tree_counter = V;
    int tree_control = 0;
    int size = V;
    double tree_weight = 0;
    std::cout << "PARALLEL MST" << std::endl;
    int start = clock();
#ifdef PRINT_CONTROL
    print_graph(gr, "starting graph");
    cout << endl;
#endif
    while (tree_counter > 1)
    {
        std::vector<int> smallest_edge(size, -1);
        std::vector<int> colors(size, -1);
        std::vector<subtree> trees(size);
        int tree_control = tree_counter;
        for (int i = 0; i < size; i++)
        {
            t_tree = subtree(i, 0);
            trees[i] = t_tree;
        }
        // a) Find minimum edge per vertex
#ifdef PRINT_CONTROL
        cout << "Find minimum edge per vertex: " << endl;
#endif
        for (int i = 0; i < size; i++)
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
#ifdef PRINT_CONTROL
        for (int i = 0; i < size; i++)
        {
            cout << i << "'s smallest edge: " << smallest_edge[i] << " (" << gr.adj[smallest_edge[i]] << "," << gr.sec_ver[smallest_edge[i]] << ") weight:" << gr.data[smallest_edge[i]] << endl;
        }
#endif
        // b) Remove mirrored edges

        for (int i = 0; i < size; i++)
        {
            if (smallest_edge[i] != -1)
            {
                int se = smallest_edge[i];
                int se_source = gr.adj[se];
                if (se_source == i)
                {
                    se_source = gr.sec_ver[se];
                }
                int sse = gr.adj[se_source];
#ifdef PRINT_CONTROL
                cout << i << "'s smallest edge: " << se << " (" << gr.adj[se] << ", " << gr.sec_ver[se] << ")" << ", source: " << se_source << ", source se: " << sse << " (" << gr.adj[sse] << ", " << gr.sec_ver[sse] << ")" << endl;
#endif
                if (i == gr.adj[smallest_edge[sse]])
                {
                    smallest_edge[i] = -1;
                }
            }
        }
#ifdef PRINT_CONTROL
        print_removed_smallest_edge(smallest_edge, size);
#endif
        // c) Initialize and propagate colors:
        for (int i = 0; i < size; i++)
        {
            if (smallest_edge[i] != -1)
            {
                int set_a = find(trees, gr.adj[smallest_edge[i]]);
                int set_b = find(trees, gr.sec_ver[smallest_edge[i]]);
                if (set_a != set_b)
                {
                    tree_weight += gr.data[smallest_edge[i]];
                    tree_union(trees, set_a, set_b);
                    tree_control--;
                }

            }
        }
        if (tree_control == tree_counter)
        {
            break;
        }
#ifdef PRINT_CONTROL
        cout << "Colors:";
#endif
        for (int i = 0; i < size; i++)
        {
#ifdef PRINT_CONTROL
            cout << find(trees, i) << " ";
#endif
            colors[i] = find(trees, i);
        }
#ifdef PRINT_CONTROL
        cout << endl;
#endif

        // d) Create new vertex ids
        std::vector<int> flags(size, 0);
        std::vector<int> exc_pre_sum(size);
        int sum = 0;
        for (int i = 0; i < size; i++)
        {
            exc_pre_sum[i] = sum;
            if (find(trees, i) == i)
            {
                flags[i] = 1;
                sum += 1;
            }
        }
#ifdef PRINT_CONTROL
        cout << "Flag:  ";
        print_arr(flags, size - 1);
        cout << "Summ:  ";
        print_arr(exc_pre_sum, size - 1);
        cout << endl;
#endif 
        // e) Count, assign, and insert new edges
        // e.1) Count 
        int nvm = sum;
        vector<int> new_nums_summ(nvm);
        vector<int> new_adjs(nvm + 1, 0);
        for (int i = 0; i < size; i++)
        {
            int color_2 = colors[i];
            for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
            {
                int color_1 = colors[gr.adj[j]];
                if (color_1 != color_2)
                {
                    new_nums_summ[exc_pre_sum[color_2]]++;
                }
#ifdef PRINT_CONTROL
                cout << "edge (" << i << "," << gr.adj[j] << ") with colors: " << color_1 << " " << color_2 << endl;
#endif
            }
        }
        int pref_sum = 0;
        for (int i = 0; i < nvm; i++) {
            pref_sum += new_nums_summ[i];
            new_adjs[i + 1] = pref_sum;
        }
#ifdef PRINT_CONTROL
        cout << "new_nums_summ: ";
        print_arr(new_nums_summ, nvm - 1);
        cout << "new_adjs: ";
        print_arr(new_adjs, nvm);
        cout << endl;
#endif
        vector<int> new_adjs_temp = new_adjs;
#ifdef PRINT_CONTROL
        print_arr(new_adjs_temp, nvm);
#endif
        for (int i = 0; i < size; i++)
        {
            int color_2 = colors[i];
            for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
            {
                int color_1 = colors[gr.adj[j]];
                if (color_1 != color_2)
                {
                    new_nums_summ[exc_pre_sum[color_2]]++;
                }
            }
        }
        int new_edge_size = new_adjs[nvm];
#ifdef PRINT_CONTROL
        cout << "new edge size: " << new_edge_size << endl;
#endif

        vector<int> new_first_edge(new_edge_size);
        vector<int> new_sec_edge(new_edge_size);
        vector<float> new_data(new_edge_size);
        for (int i = 0; i < size; i++)
        {
            int color_2 = colors[i];
            for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
            {
                int color_1 = colors[gr.adj[j]];
                if (color_1 != color_2)
                {
                    new_nums_summ[exc_pre_sum[color_2]]++;
                    new_first_edge[new_adjs_temp[exc_pre_sum[color_1]]] = exc_pre_sum[color_2];
                    new_sec_edge[new_adjs_temp[exc_pre_sum[color_1]]] = exc_pre_sum[color_1];
                    new_data[new_adjs_temp[exc_pre_sum[color_1]]] = gr.data[j];
#ifdef PRINT_CONTROL
                    cout << "trying to assign: " << new_adjs_temp[exc_pre_sum[color_1]] << endl;
#endif
                    new_adjs_temp[exc_pre_sum[color_1]]++;
                }
            }
        }

        // new graph
        gr.adj = new_first_edge;
        gr.sec_ver = new_sec_edge;
        gr.data = new_data;
        gr.nums = new_adjs;
#ifdef PRINT_CONTROL
        print_graph(gr, "new graph");
        cout << endl;
#endif
        tree_counter = tree_control;
        size = gr.nums.size() - 1;
#ifdef PRINT_CONTROL
        cout << "ITERATION END" << endl;
        std::cout << "Total weight at the end of the iteration: " << tree_weight << std::endl;
#endif

    }
#ifdef PRINT_CONTROL
    cout << (clock() - start) / CLOCKS_PER_SEC;
#endif
    std::cout << "Total weight: " << tree_weight << std::endl;

}

void print_graph(graph gr, string st)
{
    cout << st << endl;
    for (int i = 0; i < gr.data.size(); i++)
    {
        cout << "(" << gr.adj[i] << "," << gr.sec_ver[i] << ") weight: " << gr.data[i] << endl;
    }
    for (int i = 0; i < gr.nums.size(); i++)
    {
        cout << gr.nums[i] << " ";
    }
}
void print_arr(vector<int> new_adjs_temp, int size)
{
    for (int i = 0; i < size + 1; i++)
    {
        cout << new_adjs_temp[i] << " ";
    }
}
void print_removed_smallest_edge(vector<int> smallest_edge, int size)
{
    cout << "Remove mirrored edges: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << i << "'s smallest edge: " << smallest_edge[i] << endl;
    }
}