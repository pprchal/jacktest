#include <iostream>
#include "abstract_server.h"
#include "jack_server.h"
#include "server_factory.h"

using namespace std;

namespace core::snd
{
    AbstractServer* ServerFactory::create()
    {
    	AbstractServer* server = new JackServer();
        if(!server->start())
        {
            cout << "Cannot start/attach to JACK server" << endl;
        }

        return server;
    }
}

