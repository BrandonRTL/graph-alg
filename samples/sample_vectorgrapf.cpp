#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <Vectorgraph.h>
int main()
{
    int start = clock();
    graph A("3.mtx");
    std::cout << "reading time: " << (clock() - start) / CLOCKS_PER_SEC << std::endl;
    int abc;
    boruvkas_mst(A);
    std::cin >> abc;
    return 0;
}