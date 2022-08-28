#include <iostream>
#include "logger.h"

using namespace std;

namespace logging
{
    void Logger::ERROR(std::string msg)
    {
        cout << "ERROR: " << msg << endl;
    }

    void Logger::INFO(std::string msg)
    {
        cerr << msg << endl;
    }
}
