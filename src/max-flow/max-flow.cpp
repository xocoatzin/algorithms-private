#include <iostream>
#include "lib/graph.hpp"
#include "../common/io.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }



#if 0
    {
        Graph::Edge
            *e1 = graph.getEdge(0),
            *e2 = graph.getEdge(Graph::Pair(0, 1)),
            *e2b = graph.getEdge(0, 1),
            *e3 = graph.getEdge("v1", "v2");

        Graph::Vertex
            *sink = graph.getVertex("t"),
            *source = graph.getVertex("s"),
            *vn = graph.getVertex(0);
    }
#endif // 0


    // We first create the graph
    Graph graph(argv[1]);
    cout << "\nThe graph:\n";
    graph.print();

    cout << "\n\n";
    tools::wait();

    Graph::Capacity
        flow = 0;

    //Find a path from "s" to "t"
    Graph::Path
        p = graph.findPath("s", "t");
    cout << "\nFirst path:\n";
    p.print();

    do
    {
        //We find the max capacity of that path
        Graph::Capacity
            c = p.findMaxCapacity();
        cout << "\n\nThe max capacity is: " << c << endl;
        flow += c;

        //Augment the capacity into the graph
        graph.augment(p, c);

        cout << "\nThe graph:\n";
        graph.print();



        cout << "\n\n";
        tools::wait();


        p = graph.findPath("s", "t");
        cout << "\nFinding a new path:\n";
        if (p.isValid())
            p.print();
        else
            cout << "No more paths left!\n";
    }
    while (p.isValid());


    cout << "\nThe flow for this graph is: " << flow << "\n\n\n";
    tools::wait();
}
