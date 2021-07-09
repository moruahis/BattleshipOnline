#include "ClientController.h"

ClientController::ClientController()
{
    this->connected = false;
    this->ip = IPaddress();
    this->serverSocket = TCPsocket();
}

void ClientController::connect(const char* serverIp, Uint16 port)
{
    while (!connected)
    {
        if (SDLNet_ResolveHost(&this->ip, serverIp, port)==-1)
        {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            SDL_Delay(1000);
            continue;
        }

        this->serverSocket = SDLNet_TCP_Open(&this->ip);
        if (!this->serverSocket)
        {
            printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
            SDL_Delay(1000);
            continue;
        }
        else
            connected = true;
    }
}

TCPsocket ClientController::getServerSocket()
{
    return this->serverSocket;
}

void ClientController::closeConnection()
{
    SDLNet_TCP_Close(serverSocket);
}
