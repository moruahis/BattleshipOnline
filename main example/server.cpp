#include <SDL.h>
#include <SDL_net.h>
#undef main

#include <thread>
#include "ServerController.h"
#include "ClientController.h"
#include "PackageController.h"

using namespace std; 

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

    auto startClientThread = []() {
        ClientController clientController{};
        clientController.connect("127.0.0.1", 9999);
        PackageController pc;
        Package pkg{ {123,0}, requestField };
        pc.addPackageToSendQueue(pkg);
        pc.addPackageToSendQueue(pkg);
        pc.addPackageToSendQueue(pkg);
        pc.addPackageToSendQueue(pkg);
        pc.sendPackages(clientController.getServerSocket());
        clientController.closeConnection();
    };

    auto startServerThread = []() {
        ServerController serverController{};
        
        serverController.start();
        serverController.getConnections();
        serverController.printPlayerInfo(0);
        serverController.printPlayerInfo(1);
        printf("Both players connected!\n");
        PackageController pc;
        pc.receivePackages(serverController.players[0]);
        while (!pc.receivedPackagesQueue.empty())
        {
            printf("%d", pc.getFrontReceivedPackage().message);
        }

        serverController.closeConnection(0);
        serverController.closeConnection(1);
    };

    thread serverThread(startServerThread);
    thread client1Thread(startClientThread);
    thread client2Thread(startClientThread);

    serverThread.join();
    client1Thread.join();
    client2Thread.join();


    SDLNet_Quit();
    SDL_Quit();
	return 0;
}