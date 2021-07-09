#include "ServerController.h"

ServerController::ServerController()
{
    server = TCPsocket();
    done = false;
    bothPlayersConnected = false;
    socketSet = SDLNet_AllocSocketSet(2);

    printf("Starting server...\n");

    if (SDLNet_ResolveHost(&ip, NULL, 9999) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return;
    }
    server = SDLNet_TCP_Open(&ip);
    if (!server) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return;
    }
    else
        printf("Server started\n");
}

void ServerController::getConnections()
{
    while (!bothPlayersConnected) {
        /* try to accept a connection */
        TCPsocket client = SDLNet_TCP_Accept(server);
        if (!client)
        {
            printf("No connection. Retry..\n");
            SDL_Delay(1000);
            continue;
        }

        if (IPaddress* remoteIP = SDLNet_TCP_GetPeerAddress(client))
            printf("Player #%d connected: %x %d\n", (int)players.size(), SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
        else
            fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());

        players.push_back(client);
        SDLNet_TCP_AddSocket(socketSet, client);

        bothPlayersConnected = (players.size() >= 2);
    }
}

int ServerController::printPlayerInfo(int playerIndex)
{
    if (players.size() < static_cast<unsigned long long>(playerIndex) + 1)
    {
        printf("Player not connected");
        return 1;
    }
    TCPsocket* client = &players[playerIndex];

    /* get the clients IP and port number */
    IPaddress* remoteip;
    remoteip = SDLNet_TCP_GetPeerAddress(*client);
    if (!remoteip) {
        printf("Can't get player %d info\n", playerIndex);
        return 1;
    }

    /* print out the clients IP and port number */
    Uint32 ipaddr;
    ipaddr = SDL_SwapBE32(remoteip->host);
    printf("Player %d: ip = %d.%d.%d.%d, port = %hu\n", playerIndex, ipaddr >> 24,
        (ipaddr >> 16) & 0xff, (ipaddr >> 8) & 0xff, ipaddr & 0xff,
        remoteip->port);
}

void ServerController::closeConnection(int playerIndex)
{
    TCPsocket* client = &players[playerIndex];
    SDLNet_TCP_Close(*client);
}

std::vector<TCPsocket> ServerController::getPlayersSockets()
{
    return this->players;
}
