#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <Vectorgraph.h>
int main()
{
	graph A("Stranke94.mtx");
	std::vector<mst_node> b;
	int abc;
 	b = boruvkas_mst(A);
	float summ = 0;
	for (int i = 0; i < b.size(); i++)
	{
		std::cout << i << " " << b[i].parent << " " << b[i].weight << std::endl;
		summ += b[i].weight;
	}
	std::cout << "Total weight - " << summ;
	std::cin >> abc;
	return 0;
}