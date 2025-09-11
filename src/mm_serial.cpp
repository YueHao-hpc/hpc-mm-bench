#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    int N = 512;
    if (argc > 2)
    {
        try
        {
            N = stoi(argv[1]);
            if (N <= 0)
            {
                cerr << "N must be positive.\n";
                return 1;
            }
        }
        catch (...)
        { // bad input like "abc"
            cerr << "Invalid N.\n";
            return 1;
        }
    }
    return 0;
}
