#include <iostream>
#include "lib\graph.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }

    Graph graph(argv[1]);

    Graph::Edge
        *e1 = graph.getEdge(0),
        *e2 = graph.getEdge(Graph::Pair(0, 1)),
        *e2b = graph.getEdge(0, 1),
        *e3 = graph.getEdge("v1", "v2");

    Graph::Vertex
        *sink = graph.getVertex("t"),
        *source = graph.getVertex("s"),
        *vn = graph.getVertex(0);

    Graph::Path
        p = graph.findPath("s", "t");

    cout << "\nGraph:\n";
    graph.print();

    cout << "\nA path:\n";
    p.print();

    Graph::Weight
        w = p.findMaxCapacity();


}
