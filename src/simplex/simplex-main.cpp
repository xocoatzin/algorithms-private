/*********************************************************
'*                   SIMPLEX METHOD                      *
'*                   --------------                      *
'*                                                       *
'* LIST OF MAIN VARIABLES:                               *
'*                                                       *
'*  R:          MAXIMIZE = Y, MINIMIZE = N               *
'*  NV:         NUMBER OF VARIABLES OF ECONOMIC FUNCTION *
'*              (TO MAXIMIZE OR MINIMIZE).               *
'*  NC:         NUMBER OF CONSTRAINTS                    *
'*  TS:         SIMPLEX TABLE OF SIZE NC+1 x NV+1        *
'*  R1:         =1 TO MAXIMIZE, =-1 TO MINIMIZE          *
'*  R2:         AUXILIARY VARIABLE FOR INPUTS            *
'* NOPTIMAL     BOOLEAN IF FALSE, CONTINUE ITERATIONS    *
'*  XMAX:       STORES GREATER COEFFICIENT OF ECONOMIC   *
'*              FUNCTION.                                *
'*  RAP         STORES SMALLEST RATIO > 0                *
'*  V:          AUXILIARY VARIABLE                       *
'*  P1,P2:      LINE, COLUMN INDEX OF PIVOT              *
'*  XERR:       BOOLEAN IF TRUE, NO SOLUTION.            *
'* ----------------------------------------------------- *
'* PROBLEM DESCRIPTION:                                  *
'* A builder of houses can make 3 kinds of them with     *
'* various profits: 15000$, 17000$ and 20000$.           *
'* Each kind must respect following conditions:          *
'* 1) for supply reasons, the number of houses of kind 2 *
'*    built each month must not exceed the number of     *
'*    kind 3 by more than two.                           *
'* 2) for staff reasons, the buider can make each month  *
'*    up to 5, 5 and 3, respectively of kinds 1, 2 and 3.*
'* 3) for organisation reasons, the builder can make up  *
'*    to 8 houses monthly of kinds 1,2 and 3, respecti-  *
'*    vely in the proportions of 3, 2 and 1.             *
'* The builder, having these data, wants to maximize his *
'* monthly profit by knowing the number oh houses of     *
'* each kind to build monthly.                           *
'* ----------------------------------------------------- *
'* SAMPLE RUN:                                           *
'* (Maximize 15 X1 + 17 X2 + 20 X3 with conditions:      *
'*                    X2 -   X3 <= 2                     *
'*           3 X1 + 3 X2 + 5 X3 <= 15                    *
'*           3 X1 + 2 X2 +   X3 <= 8     )               *
'*                                                       *
'* LINEAR PROGRAMMING                                    *
'*                                                       *
'* MAXIMIZE ? Y                                          *
'*                                                       *
'* NUMBER OF VARIABLES OF ECONOMIC FUNCTION ? 3          *
'*                                                       *
'* NUMBER OF CONSTRAINTS ? 3                             *
'*                                                       *
'* INPUT COEFFICIENTS OF ECONOMIC FUNCTION:              *
'*       #1 ? 15                                         *
'*       #2 ? 17                                         *
'*       #3 ? 20                                         *
'*       Right hand side ? 0                             *
'*                                                       *
'* CONSTRAINT #1:                                        *
'*       #1 ? 0                                          *
'*       #2 ? 1                                          *
'*       #3 ? -1                                         *
'*       Right hand side ? 2                             *
'*                                                       *
'* CONSTRAINT #2:                                        *
'*       #1 ? 3                                          *
'*       #2 ? 3                                          *
'*       #3 ? 5                                          *
'*       Right hand side ? 15                            *
'*                                                       *
'* CONSTRAINT #3:                                        *
'*       #1 ? 3                                          *
'*       #2 ? 2                                          *
'*       #3 ? 1                                          *
'*       Right hand side ? 8                             *
'*                                                       *
'* RESULTS:                                              *
'*                                                       *
'*       VARIABLE #1: 0.333333                           *
'*       VARIABLE #2: 3.000000                           *
'*       VARIABLE #3: 1.000000                           *
'*                                                       *
'*       ECONOMIC FUNCTION: 76.000000                    *
'*                                                       *
'* (Building monthly 1/3, 3 and 1 house(s) of kinds 1,2, *
'*  3, the builder can make a monthly profit of 76000$). *
'* ----------------------------------------------------- *
'* REFERENCE:                                            *
'* Mod�les pratiques de d�cision Tome 2, By Jean-Pierre  *
'* Blanger, PSI Editions, France, 1982.                  *
'*                                                       *
'*                C++ Release 1.0 By J-P Moreau, Paris.  *
*                          (www.jpmoreau.fr)             *
'********************************************************/

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
        problem.Pivot();
        problem.Formula();
        problem.Optimize();

        problem.print();
        tools::wait();
    } while (!problem.isOptimal);

    problem.result();
    tools::wait();

    return 0;
}
