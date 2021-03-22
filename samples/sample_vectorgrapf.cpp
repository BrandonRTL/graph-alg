#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <Vectorgraph.h>
using namespace std;
int main()
{
    int is_test = 0;
    graph A("6.mtx");
    if (is_test) {
        for (int i = 0; i < A.data.size(); i++)
        {
            cout << "(" << A.adj[i] << "," << A.sec_ver[i] << ") weight: " << A.data[i] << endl;
        }
    }
    int abc;
    boruvkas_mst(A);
    boruvkas_parallel(A);
    std::cin >> abc;
    return 0;
}