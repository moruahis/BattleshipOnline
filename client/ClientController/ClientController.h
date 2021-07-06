#ifndef CLIENTCONTROLLERH
#define CLIENTCONTROLLERH

#include <SDL.h>
#include <SDL_net.h>
#include <cstdio>
#include <vector>
#include "Package.h"

class ClientController
{
	TCPsocket serverSocket;
	IPaddress ip;
	bool connected;

public:
	int start();
	void connect(const char* serverIp, Uint16 port);
	TCPsocket getServerSocket();
	void closeConnection();
};

#endif
