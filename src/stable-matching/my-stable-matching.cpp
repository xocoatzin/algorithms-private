#include <iostream>
#include "tools.h"
#include "datastructures.h"

using namespace tools;
using namespace stable_matching;
using namespace std;

int main(int argc, char *argv[])
{
    // *********************************************************************************
    // Don't modify this section
    typedef Population::Person Person;
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        return -1;
    }

    Parser parser(argv[1]);
    Preferences preferences = parser.getPreferences();
    Members members =  parser.getMembers();
    Population population(members, preferences);
    // *********************************************************************************

    // =================================================================================
    // The algorithm starts here!
    // =================================================================================

    // Notes:
    // All the information you need is stored in {population}
    // There is a list of single men you can use in {population.single_men}
    // You can extract them using {population.nextSingleMan()}, this will make them leave the list
    // If a girl dumps a man, the man returns to the list of single men automatically.

    // 1) Loop trough single men
    // Hints:
    // Get the number of single men remaining with:   population.single_men.size()
    //
    // Example:
    // while (population.single_men.size() > 0)
    // {
    //     ...
    // }

    // 2) Get the next single man
    // Hint:
    // Use:  Person *single_guy = population.nextSingleMan();

    // 3) Get the next preference of that man
    // Hints:
    // Use: Person *girl = single_guy->nextPreference();
    // Each time you call this function, you'll get the next preference for 
    // that man, in order

    // 4) Make the man propose
    // Hints:
    // Use: Person::Response response = single_guy->propose(girl);
    // {response} contains the answer of the girl,
    // If the girl accepts, the response will be:  response == Person::ACCEPT_PROPOSAL
    //   otherwise:                                response == Person::REJECT_PROPOSAL

    // 5) Repeat until a girl accepts
    // Hints:
    // do {
    //     ** get next preference and propose her**
    // } while (response == Person::REJECT_PROPOSAL);

    // 6) Show the results
    // Hints:
    // Print the preference lists with:   population.print();
    // Wait for the user to press [enter] with:   cin.get();




    return 0;
}
