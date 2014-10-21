#include <iostream>
#include "stable-matching-tools.h"
#include "datastructures.h"

using namespace tools;
using namespace stable_matching;
using namespace std;

int main(int argc, char *argv[])
{
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

    unsigned int round = 0;
    while (population.single_men.size() > 0)
    {
        cout << "===================================================\n\nRound " << ++round << endl << endl;

        Person *single_guy = population.nextSingleMan();

        if (!single_guy)
        {
            cout << "Person not found!" << endl;
            continue;
        }

        cout << "Now it's the turn of: " << single_guy->name() << endl;

        Person::Response response;
        do
        {
            Person *girl = single_guy->nextPreference();

            if (!girl)
            {
                cout << "   There's no more girls in the preference list of " << single_guy->name() << endl;
                break;
            }

            cout << "   Proposing to " << girl->name() << ": ";
            response = single_guy->propose(girl);
            if (response == Person::ACCEPT_PROPOSAL) cout << " and accepts!" << endl;
            else if (response == Person::REJECT_PROPOSAL) cout << " and rejects " << single_guy->name() << "!" << endl;
        }
        while (response == Person::REJECT_PROPOSAL);

        cout << "\n\n" << endl;
        population.print();
        cout << "\n\nEnd of round\n" << endl;
        tools::wait();
    }

    cout << "STOP, Everyone matched!\n" << endl;
    return 0;
}
