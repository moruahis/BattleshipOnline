#include <SDL.h>
#include <SDL_net.h>
#undef main

#include "ServerController.h"

int SDLInit() {
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        return 2;
    }
    return 0;
}

int main()
{
    SDLInit();
    ServerController serverController{};

    serverController.start();
    serverController.getConnections();
    serverController.printPlayerInfo(0);
    serverController.printPlayerInfo(1);

    printf("Both players connected! Shutting down\n");

    serverController.closeConnection(0);
    
    SDLNet_Quit();
    SDL_Quit();
	return 0;
}