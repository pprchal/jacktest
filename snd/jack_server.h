#pragma once

#include <jack/jack.h>
#include <jack/midiport.h>

#include "abstract_server.h"

namespace core::snd
{
    class JackServer : public AbstractServer 
    {
        public:
            JackServer();
            virtual ~JackServer();
            virtual bool start();
            virtual void close();

        private:
            char* j_client_name;
            jack_client_t* j_client;
            jack_transport_state_t j_state = JackTransportStopped;
            jack_port_t* j_midi_port;

            void wireup();

            jack_transport_state_t t_state;
            
            // JACK callbacks
            static int process_midi(jack_nframes_t nframes, void *arg);
            static int process_audio(jack_nframes_t nframes, void *arg);
            static void shutdown (void *arg);
    };
}
