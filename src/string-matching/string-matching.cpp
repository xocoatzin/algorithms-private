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
            for (unsigned int i = 0; i < str.length() - pattern.length() + 1; i++)
            {
                print(str, pattern, i);

                bool match = true;
                for (unsigned int j = 0; j < pattern.length(); j++)
                {
                    if (str.at(j + i) != pattern.at(j))
                    {
                        match = false;
                        break;
                    }
                }

                if (match) cout << "   matches at char " << i << endl << endl;
                else cout << endl << endl;

                tools::wait();
            }
        }

        cout << "\n\nKnuth-Morris-Pratt:\n===================================================\n\n";
        {
            KMPPrefixTable table(pattern);
            unsigned int
                n = str.length(),
                m = pattern.length(),
                i = 0;

            print(str, pattern, 0);
            cout << endl << endl;

            for (unsigned int j = 0; j < n; j++)
            {
                while (i > 0 && pattern.at(i) != str.at(j))
                {
                    print(str, pattern, j - i + 1);
                    cout << endl << endl;

                    tools::wait();

                    i = table[i];
                }

                if (pattern.at(i) == str.at(j))
                {
                    i++;
                }

                if (i == m)
                {
                    unsigned int pos = (j - i + 1);

                    print(str, pattern, j - i + 1);
                    cout << "   matches at char " << pos << endl << endl;

                    i = table[i];

                    tools::wait();
                }
            }
        }

        cout << "\n\nBoyer-Moore-Horspool:\n===================================================\n\n";
        {
            BMHTable table(pattern);
            unsigned int
                n = str.length(),
                m = pattern.length(),
                offset = 0,
                i = m - 1,
                j = i + offset;

            print(str, pattern, 0);
            cout << endl << endl;

            while (offset + m < n)
            {
                if (str.at(j) == pattern.at(i))
                {
                    if (i == 0)
                    {
                        cout << "   found at " << offset << endl << endl;

                        tools::wait();

                        offset += 1;
                        i = m - 1;
                        j = i + offset;
                    }
                    else
                    {
                        i--;
                        j--;
                    }
                }
                else
                {
                    offset += table.at(str.at(j));
                    i = m - 1;
                    j = i + offset;

                    cout << endl << endl;
                    tools::wait();

                    cout << endl << endl;
                    print(str, pattern, offset);
                }
            }
        }
    }

    cout << "End" << endl;
    tools::wait();

    return 0;
}
