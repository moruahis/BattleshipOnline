#include <SDL.h>
#include <SDL_net.h>
#undef main

#include <thread>
#include "../headers/server/ServerController/ServerController.h"
#include "../headers/server/GameLogicController/GameLogicController.h"
#include "../headers/common/PackageController/PackageController.h"
#include "../headers/common/Package.h"

#include "../headers/client/ClientController/ClientController.h"
#include "../headers/client/GameClientController/GameClientController.h"



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
	/*
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
	*/

	ServerController serverController;
	serverController.getConnections();

	PackageController packageController{};
	packageController.addPackageToSendQueue(Package{ {1}, infoBothConnected });
	packageController.sendPackages(serverController.getPlayersSockets()[0]);
	packageController.addPackageToSendQueue(Package{ {1}, infoBothConnected });
	packageController.sendPackages(serverController.getPlayersSockets()[1]);

	GameLogicController gameLogicController{};
	packageController.receivePackages(serverController.getPlayersSockets()[0]);
	while (!packageController.receivedPackagesQueue.empty())
	{
		printf("%d\n", packageController.getFrontReceivedPackage().message);
	}
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}