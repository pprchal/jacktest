#!/bin/sh

g++ jack_server.cc jacktest.cc abstract_server.cc server_factory.cc -o jacktest -l jack

