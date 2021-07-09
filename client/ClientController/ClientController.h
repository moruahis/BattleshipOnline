#ifndef CLIENTCONTROLLERH
#define CLIENTCONTROLLERH

#include <SDL.h>
#include <SDL_net.h>
#include <cstdio>
#include <vector>
#include "../../common/Package.h"

class ClientController
{
	TCPsocket serverSocket;
	IPaddress ip;
	bool connected;

public:
	bool bothPlayersConnected;
	ClientController();
	void connect(const char* serverIp, Uint16 port);
	TCPsocket getServerSocket();
	void closeConnection();
};

#endif
