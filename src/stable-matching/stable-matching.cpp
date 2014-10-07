#include <iostream>
#include "tools.h"
#include "datastructures.h"

using namespace tools;
using namespace stable_matching;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error! you need to include the input file in the command line" << std::endl;
        return -1;
    }

    // Here we read the input file and extract the information
    Parser parser;
    parser.parse(argv[1]);

    // You can write your code bellow, feel free to replace the lines below
    // ********************************************************************

    // Getting access to the information read from the input file
    Members members = parser.getMembers();                                    // We get all the members
    Preferences preferences = parser.getPreferences();                        // We get the preference list of the members

    // Printing information
    std::cout << "\nThe members are:" << std::endl;
    members.print();
    std::cout << "\nNumber of elements: " << members.size() << std::endl;

    std::cout << "\nThe preference list (with names):" << std::endl;
    members.print(preferences);
    std::cout << "\nThe preference list (with id's):" << std::endl;
    preferences.print();
    std::cout << "\nNumber of elements: " << preferences.size() << std::endl;

    // You can use a filter when getting the information
    std::cout << "\nThe members whose index starts with 'm':" << std::endl;
    Members men = parser.getMembers("m"); // returns the members whose ID starts with 'm'
    men.print();

    Preferences men_preferences = parser.getPreferences("m");
    members.print(men_preferences);


    // Relationships
    Relationships relations;
    std::cout << "\nCreating some relationships:" << std::endl;

    relations.add("m1", "f1");
    relations.add("m3", "f2"); // m3
    relations.add("m2", "f4");
    relations.add("m3", "f3"); // This relationship is not created (m3 is not single)

    std::cout << "\nPrinting the relationships (with names):" << std::endl;
    members.print(relations);
    std::cout << "\nPrinting the relationships (with ID):" << std::endl;
    relations.print();

    std::cout << "\nTesting relationships:" << std::endl;
    relations.exist("m1", "f1"); // yes
    relations.exist("f1", "m1"); // yes (works in any order)
    relations.exist("f1", "m3"); // no
    relations.exist("x1", "y1"); // no (indices don't exist)
    std::cout << "Is {m1} single? " << (relations.is_single("m1") ? "yes" : "no") << std::endl;  // Returns false (already in a relationship with 'f1')
    std::cout << "Is {m4} single? " << (relations.is_single("m4") ? "yes" : "no") << std::endl;  // Returns true ('m4' is still single)

    std::cout << "\nReplacing relationships:" << std::endl;
    relations.free("m1"); //both "m1" and "f1" become free
    relations.add("m1", "f3"); // Now 'm1' can have a relationship with someone else
    relations.add("f1", "m4");

    std::cout << "\nPrinting the new relationships:" << std::endl;
    members.print(relations);


    // Iterating a list
    std::cout << "\nIterating the member list and printing females (start with 'f'):" << std::endl;
    for (Members::iterator it = members.begin(); it != members.end(); it++)
    {
        // {it->first} is the key
        // {it->second} is the value (name)
        // Here we use the arrow operator! (->)
        if (beginsWith(it->first, "f"))           // if the key starts with 'f'
            std::cout << it->second << std::endl; // we print the name
    }

    // You can iterate using C++11 style (shorter)
    for (auto &it : preferences)
    {
        // {it.first} is the key
        // {it.second} is the value (a list of ID's, the type is std::vector<std::string>)
        // NOTE now 'it' uses dot (.) operator instead of arrow (->)!
        if (beginsWith(it.first, "f")) // if the key starts with 'f'
        {
            auto list = it.second; // this is the preference list!
            auto name = members.get(it.first); // this is the name of the member
            std::cout << "\nPreferences for " << name << ": " << std::endl;

            // We can iterate the preference list in the same way!
            // {option} is the ID (a std::string)!
            int i = 0;
            for (auto &option : list)
            {
                // Do something with the options?
                auto option_name = members.get(option); // this is the name of the member
                std::cout << "    " << i++ << ": " << option << ", " << option_name << std::endl;
            }
        }
    }

    // Tou can also get the preference of any member by ID
    std::string key = "m1";
    auto pref_m1 = preferences.get(key); // preference list
    std::cout << "\nPreferences for " << members.get(key) << ": " << std::endl;
    int i = 0;
    for (auto &option : pref_m1)
    {
        auto option_name = members.get(option); // this is the name of the member
        std::cout << "    " << i++ << ": " << option_name;
        if (relations.is_single(option)) //is the option single?
            std::cout << " IS SINGLE!!!" << std::endl;
        else
            std::cout << " not single, keep trying..." << std::endl;
    }


    return 0;
}
