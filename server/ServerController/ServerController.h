#ifndef SERVERCONTROLLERH
#define SERVERCONTROLLERH

#include <SDL.h>
#include <SDL_net.h>
#include <cstdio>
#include <vector>
#include "Package.h"

class ServerController {
public:
    TCPsocket server;
    std::vector<TCPsocket> players;
    IPaddress ip;
    SDLNet_SocketSet socketSet;

    bool done;
    bool bothPlayersConnected;

    int start();
    void getConnections();
    int printPlayerInfo(int playerIndex);
    void closeConnection(int playerIndex);
    std::vector<TCPsocket> getPlayersSockets();
};

#endif
