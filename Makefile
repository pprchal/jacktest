CFLAGS ?= -Wall
CC = g++ -std=c++2a
LIBS = -lm -lpthread -ljack

default: all

abstract_server: 
	$(CC) -g -c snd/abstract_server.cc

jack_server: 
	$(CC) -g -c snd/jack_server.cc

server_factory: 
	$(CC) -g -c snd/server_factory.cc

logger: 
	$(CC) -g -c logger.cc

jacktest: 
	$(CC) -g -c jacktest.cc

main: 
	$(CC) -g abstract_server.o jack_server.o server_factory.o logger.o jacktest.o -o jacktest $(LIBS)

	
# $(CC) $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS) $(LIBS) -shared -fPIC -o $@


clean:
	rm -f snd/abstract_server.o snd/jack_server.o snd/server_factory.o logger.o jacktest.o


all: abstract_server jack_server server_factory logger jacktest main

