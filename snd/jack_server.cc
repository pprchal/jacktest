#include <jack/jack.h>
#include <jack/midiport.h>
#include <iostream>
#include <iomanip>

#include "jack_server.h"

namespace core::snd
{
    bool JackServer::start()
    {
        jack_status_t status;
        j_client = jack_client_open("testik", JackNullOption, &status);
        if (j_client == NULL)
        {
            // LOG("jack_client_open() failed, status = 0x%2.0x\n", status);
            log->INFO("Unable to connect to JACK server: jack_client_open()");

            if (status & JackServerFailed)
            {
                log->ERROR("Err");
            }

            return false;
        }

        log->INFO("STARTED");

        if (status & JackServerStarted)
        {
            log->INFO("JACK server started");
        }

        if (status & JackNameNotUnique)
        {
            j_client_name = jack_get_client_name(j_client);
            log->ERROR("JackNameNotUnique"); // "jack-client name: `%s'\n", j_client_name);
        }

        // MIDI callback
        jack_set_process_callback(
            j_client,
            process_audio,
            this);

        // Audion callback
        // jack_set_process_callback(
        //     j_client,
        //     process_audio,
        //     this);

        jack_on_shutdown(
            j_client,
            shutdown,
            this);

        wireup();
        return true;
    }

    void JackServer::wireup()
    {
        const char *portName = "portik";

        j_midi_port = jack_port_register(
            j_client,
            portName,
            JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsInput,
            0);

        if (j_midi_port == nullptr)
        {
            log->ERROR("Cannot register JACK port");
            return;
        }
        log->INFO("Port registered");

        int status = jack_activate(j_client);
        log->INFO("jack_activate()");

        // TODO: connect ports
        // int status = jack_connect(j_client, jack_port_name(j_port), portName);
        // LOG("???");
        // if (status == 0)
        // {
        //     LOG("Port connection OK");
        // }
        // else
        // {
        //     fprintf(stderr, "jack_connect()=%i\n", status);
        // }
    }

    JackServer::JackServer()
    {
        log->INFO("Using JACK server");
    }

    JackServer::~JackServer()
    {
        jack_client_close(j_client);
        j_client = nullptr;
        log->INFO("Disposing JACK server");
    }

    const char* formatJackState(jack_transport_state_t transportState)
    {
        switch(transportState)
        {
            case JackTransportStopped: return "JackTransportStopped";
            case JackTransportRolling: return "JackTransportRolling";
            case JackTransportLooping: return "JackTransportLooping";
            case JackTransportStarting: return "JackTransportStarting";
            case JackTransportNetStarting: return "JackTransportNetStarting";
        }

        return "formatJackState - Unknown";
    }

    int JackServer::process_midi(jack_nframes_t nframes, void *arg)
    {
        auto me = static_cast<JackServer *>(arg);
        auto port_buf = jack_port_get_buffer(me->j_midi_port, nframes);

        jack_midi_event_t in_event;
        jack_nframes_t event_count = jack_midi_get_event_count(port_buf);

        jack_position_t pos;
        auto ts = jack_transport_query(me->j_client, &pos);
        if(me->t_state != ts)
        {
            printf(" state: %s  ->   %s\n, pos.frame: %i", 
                formatJackState(me->t_state), 
                formatJackState(ts),
                pos.frame
            );
            me->t_state = ts;
        }

        if (event_count > 1)
        {
           // printf(" midisine: have %d events\n", event_count);
            for (int i = 0; i < event_count; i++)
            {
                jack_midi_event_get(&in_event, port_buf, i);
                // printf("    event %d time is %d. 1st byte is 0x%x\n", i, in_event.time, *(in_event.buffer));
            }
        }
        jack_midi_event_get(&in_event, port_buf, 0);
        return 0;
    }
    
    int JackServer::process_audio(jack_nframes_t nframes, void *arg)
    {
        auto me = static_cast<JackServer *>(arg);
        auto port_buf = jack_port_get_buffer(me->j_midi_port, nframes);
        auto now = std::time(0);
        auto tm = localtime(&now);
        jack_position_t pos;
        auto cur_state = jack_transport_query(me->j_client, &pos);

        if(me->t_state != cur_state)
        {
            printf("state-changed: %s  ->   %s, pos.frame: %i, time[%i:%i]", 
                formatJackState(me->t_state), 
                formatJackState(cur_state),
                pos.frame,
                tm->tm_min, tm->tm_sec
            );
            me->t_state = cur_state;
        }

        printf("state: %s, pos.frame: %i, time[%i:%i]\n", 
            formatJackState(cur_state),
            pos.frame,
            tm->tm_min, tm->tm_sec
        );

        return 0;
    }

    void JackServer::close()
    {
        jack_client_close(j_client);
    }

    void JackServer::shutdown(void *arg)
    {
        auto me = static_cast<JackServer *>(arg);
        me->log->INFO("shutdown() request from JACK server");
    }
}