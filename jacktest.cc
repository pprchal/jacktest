#include <iostream>
#include "snd/server_factory.h"

using namespace std;
using namespace core::snd;

int main()
{
	cout << "JACK test console" << endl;
	auto server = ServerFactory::create();
	cout << "<ENTER> to stop" << endl;

	std::string s;
	std::getline(std::cin, s);
	delete server;

	return 0;
}
