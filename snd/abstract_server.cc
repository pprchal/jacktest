#include "abstract_server.h"
#include "../logger.h"

using namespace std;

namespace core::snd
{
    AbstractServer::AbstractServer()
    {
        log = new logging::Logger();
    }
}

