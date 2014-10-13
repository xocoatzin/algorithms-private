#include <iostream>
#include <string>

#include "../common/string.h"
#include "string-matching-tools.h"

using namespace std;
using namespace string_matching;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error! you need to include the input file in the command line" << endl;
        tools::wait();
        return -1;
    }

    tools::Reader reader(argv[1]);

    string line;
    size_t line_id;
    while (reader.getLine(line, line_id))
    {
        vector<string> tokens = tools::split(line, "->");

        if (tokens.size() < 2)
        {
            cout << " Skipping line " << line_id << endl;
            continue;
        }

        string
            str = tokens[1],
            pattern = tokens[0];

        cout << "\n\nNaive matching:\n===================================================\n\n";
        {
            // 1) Start at possition 0 and increase by 1 on each iteration. Loop.
            // Hint:
            //     Get the length of a string with:   string_name.length()
            for (unsigned int offset = 0; /* WRITE ME! */ ; offset++)
            {
                print(str, pattern, offset);

                // 2) For each character {j} on the pattern. Loop.
                for (/* WRITE ME! */)
                {
                    // 3) Compare character {i} to character {j}
                    // Hint:
                    //     Use: str.at(j + offset) == pattern.at(j)
                    //     The offset comes from the starting position in step (1)
                }

                // 4) Have you found the pattern?
                // If you reached the end of the pattern and all characters matched, you found the solution
                // If you found a character that doesn't match, you can skip to the next position (use 'break')

                if (/* WRITE ME! */)
                    cout << "   IS A MATCH!" << endl << endl;
                else
                    cout << endl << endl;

                tools::wait(); // wait for user to press [enter]
            }
        }

        cout << "\n\nKnuth-Morris-Pratt:\n===================================================\n\n";
        {
            KMPPrefixTable table(pattern);
            unsigned int
                n = str.length(),
                m = pattern.length(),
                i = 0;
            
            /*
             *  Note: In c++, the indices start in 0.
             *        The comments on this document are different from the slides to make this consideration
             */

            // for j = 0 upto n step 1 do   (Note j = 0)
            for (/* WRITE ME! */)
            {
                // while i > 0 and P[i] != T[j] do.   (Note P[i])
                while (/* WRITE ME! */)
                {
                    // Skip using prex table
                    i = table[i];
                }

                // if P[i] = T[j] then   (Note P[i])
                if (/* WRITE ME! */)
                {
                    // Next character matches
                }

                // if i = m then
                if (/* WRITE ME! */)
                {
                    // OUTPUT(j - m + 1)   (Note + 1)
                    // Look for next match
                }
            }
        }

        cout << "\n\nBoyer-Moore-Horspool:\n===================================================\n\n";
        {
            BMHTable table(pattern);
            unsigned int
                n = str.length(),             // Length of the string
                m = pattern.length(),         // Length of the pattern
                offset = 0,                   // Offset (position of the pattern)
                i = m - 1,                    // Character to test in pattern
                j = i + offset;               // Character to test in string

            // 1) Repeat until we reach the end of the string
            // 
            // offset + m < n
            while (/* WRITE ME! */)
            {
                // 2) Compare character {i} to character {j}
                // Hint:
                //     Use: str.at(j) == pattern.at(i)
                if (/* WRITE ME! */)
                {
                    // 3A) The character matches, test next one?
                    if (i < 0)
                    {
                        // We haven't reached the first character
                        // modify {i} and {j} so we would test the next character (reverse order)
                    }
                    else
                    {
                        // We reached the first character, all matched, we found it!!
                        // Print something here!


                        // exit
                        break;
                    }
                }
                else
                {
                    // 3B) The character didn't match, increase the offset
                    // Hint:
                    //     Use the table to know how much to increment the offset:   unsigned char increment = table.at(str.at(j));

                    // Reset indices
                    i = m - 1;
                    j = i + offset;
                }
            }
        }

    } // while

    cout << "End" << endl;
    tools::wait();

    return 0;
}
