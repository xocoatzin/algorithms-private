#include <iostream>
#include "pilal.h"
#include "simplex.h"

using namespace std;
using namespace pilal;
using namespace optimization;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }


    Simplex simplex("Simplex Instance");

    try
    {
        simplex.load_problem(argv[1]);
        simplex.log();
        // =================================================================================

        simplex.solve();



        // =================================================================================

        if (simplex.must_be_fixed())
        {
            std::cout << "Problem formulation is incorrect." << std::endl;
            return -1;
        }

        // Print the results
        if (simplex.has_solutions())
        {
            if (!simplex.is_unlimited())
                simplex.print_solution();
            else
                std::cout << "Problem is unlimited." << std::endl;
        }
        else
        {
            std::cout << "Problem is overconstrained." << std::endl;
        }
    }
    catch (DataMismatchException c) {
        std::cout << "Error: " << c.error << std::endl;
    }

    return 0;
}
