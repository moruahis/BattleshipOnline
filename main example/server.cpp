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
	packageController.addPackageToSendQueue(Package{ {0}, setIndex }, 0);
	packageController.addPackageToSendQueue(Package{ {1}, infoBothConnected }, 0);
	packageController.sendPackages(serverController.getPlayersSockets()[0], 0);
	packageController.addPackageToSendQueue(Package{ {1}, setIndex }, 1);
	packageController.addPackageToSendQueue(Package{ {1}, infoBothConnected }, 1);
	packageController.sendPackages(serverController.getPlayersSockets()[1], 1);

	GameLogicController gameLogicController{};
	bool done = false;
	bool stage2 = false;
	int timer = 0;
	while (!done)
	{
		if (timer++ < 1000)
			continue;
		else
			timer = 0;
		if (!stage2 && gameLogicController.shipsPlaced[0] > 9 && gameLogicController.shipsPlaced[1] > 9)
		{
			Package package{ {0}, battleBegins };
			for (int pl = 0; pl < 2; pl++)
			{
				packageController.addPackageToSendQueue(package, pl);
				packageController.sendPackages(serverController.getPlayersSockets()[pl], pl);
			}
			stage2 = true;
		}
		for (int pl = 0; pl < 2; pl++)
		{
			packageController.receivePackages(serverController.getPlayersSockets()[pl], pl);
			while (!packageController.receivedPackagesQueue[pl].empty())
			{
				Package response = gameLogicController.proceedRequest(packageController.getFrontReceivedPackage(pl));
				if (response.message == timeToSendPackages)
				{
					packageController.sendPackages(serverController.getPlayersSockets()[pl], pl);
					continue;
				}
				else if (response.message != emptyPackage)
				{
					packageController.addPackageToSendQueue(response, pl);
				}
			}
		}
	}
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}