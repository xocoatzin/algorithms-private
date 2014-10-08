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

    // 1) Loop trough single men
    // Hints:
    // Get the number if single men remaining with:   population.single_men.size()
    // Get the next single men with:   Person *single_guy = population.nextSingleMan();
    // If a girl dumps a man, the man returns to the list of single men automatically

    // 2) Each man proposes girls in order of preference until one accepts
    // Hints:
    // Get the next preference of a man with:   Person *girl = single_guy->nextPreference();
    // Propose a girl with:   Person::Response response = single_guy->propose(girl);
    // If the girl accept, the response will be:  response == Person::ACCEPT_PROPOSAL
    //   otherwise:                               response == Person::REJECT_PROPOSAL

    // 3) Show the results
    // Hints:
    // Print the preference lists with:   population.print();
    // Wait for the user to press [enter] with:   cin.get();




    return 0;
}
