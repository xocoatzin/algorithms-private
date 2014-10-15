#include <iostream>
#include "pilal.h"
#include "simplex.h"

using namespace std;
using namespace pilal;
using namespace optimization;

int main(int argc, char *argv[])
{
    if (argc == 2) {
        Simplex problem("Simplex Instance");

        try {
            problem.load_problem(argv[1]);


            // Solve
            problem.solve();
            std::cout << std::endl;

            if (problem.must_be_fixed()) {
                std::cout << "Problem formulation is incorrect." << std::endl;
                return 1;
            }

            if (problem.has_solutions()) {
                if (!problem.is_unlimited())
                    problem.print_solution();
                else
                    std::cout << "Problem is unlimited." << std::endl;

            }
            else {
                std::cout << "Problem is overconstrained." << std::endl;
            }

        }
        catch (DataMismatchException c) {
            std::cout << "Error: " << c.error << std::endl;
        }

        return 0;
    }
    else {
        std::cout << "Error: omitted problem file." << std::endl;
        return 1;
    }

    std::cout << "Quitting ..." << std::endl;
    return 0;
}
