#include <SDL.h>
#include <SDL_net.h>
#undef main

#include <iostream>
#include <thread>

#include "../headers/client/ClientController/ClientController.h"
#include "../headers/client/GameClientController/GameClientController.h"
#include "../headers/common/PackageController/PackageController.h"
#include "../headers/common/Package.h"

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
	ClientController clientController;
	clientController.connect("127.0.0.1", 9999);

	PackageController packageController{};
	while (!clientController.bothPlayersConnected)
	{
		printf("waiting for both players to connect\n");
		packageController.receivePackages(clientController.getServerSocket());
		while (!packageController.receivedPackagesQueue.empty())
		{
			Package package = packageController.getFrontReceivedPackage();
			if (package.message == infoBothConnected)
			{
				clientController.bothPlayersConnected = package.data[0];
			}
			else
				continue;
		}
	}

	GameClientController gameClientController{};
	Package request = gameClientController.makeShot(0, 0);
	packageController.addPackageToSendQueue(request);
	packageController.sendPackages(clientController.getServerSocket());
}
