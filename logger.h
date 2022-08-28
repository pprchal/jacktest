#pragma once

#include <string>

namespace logging
{
    class Logger
    {
        public:
            void ERROR(std::string msg);
            void INFO(std::string msg);
    };
}