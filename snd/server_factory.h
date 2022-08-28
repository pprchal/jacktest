#include "abstract_server.h"
#include "jack_server.h"

namespace core::snd
{
    class ServerFactory
    {
        public:
            static AbstractServer* create();
    };
}