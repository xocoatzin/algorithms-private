#include <iostream>
#include "lib\graph.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    std::cout << "Hello World!";

    Graph graph(2, 1);
    int vtxIdx1 = graph.addVtx();
    int vtxIdx2 = graph.addVtx();
    graph.addEdges(vtxIdx1, vtxIdx2, 2, 2);

    graph.maxFlow();
}
