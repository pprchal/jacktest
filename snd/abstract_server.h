#pragma once

#include "../logger.h"

using namespace std;

namespace core::snd
{
    class AbstractServer
    {
        protected:
            logging::Logger* log;

        public:
            AbstractServer();
            virtual bool start() = 0;
            virtual void close() = 0;
    };
}
