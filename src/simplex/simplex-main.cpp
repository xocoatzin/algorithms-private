#include <iostream>
#include "simplex-tools.h"

using namespace tools;
using namespace simplex;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }

    Parser parser(argv[1]);
    Problem problem = parser.getProblem();

    if (problem.objective == Problem::VOID)
    {
        cerr << "Error! There was an error on the formulation of the problem" << endl;
        return -1;
    }

    problem.print();
    tools::wait();

    do
    {
        cout << "Finding next pivot" << endl;
        problem.Pivot();
        cout << "Formulating" << endl;
        problem.Formula();
        cout << "Optimize" << endl;
        problem.Optimize();

        problem.print();
        if (!problem.isOptimal) cout << "Solution not yet optimal, running a new iteration" << endl;
        tools::wait();
    } while (!problem.isOptimal);

    cout << "The results found:" << endl;
    problem.result();
    tools::wait();

    return 0;
}
