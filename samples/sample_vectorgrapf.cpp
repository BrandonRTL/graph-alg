#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <Vectorgraph.h>
int main()
{
	graph A("3.mtx");
	int abc;
	boruvkas_mst(A);	
	std::cin >> abc;
	return 0;
}