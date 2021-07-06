#include "ClientController.h"

int ClientController::start()
{
    return 0;
}

void ClientController::connect(const char* serverIp, Uint16 port)
{
    while (!connected)
    {
        if (!SDLNet_ResolveHost(&this->ip, serverIp, port))
        {
            printf("Can't resolve host\n");
            SDL_Delay(1000);
        }

        this->serverSocket = SDLNet_TCP_Open(&this->ip);
        if (!this->serverSocket)
        {
            printf("No connection. Retrying\n");
            SDL_Delay(1000);
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
