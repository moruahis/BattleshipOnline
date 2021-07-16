#include <SDL.h>
#include <SDL_net.h>
#undef main

#include <iostream>
#include <thread>

#include "../headers/client/ClientController/ClientController.h"
#include "../headers/client/GameClientController/GameClientController.h"
#include "../headers/common/PackageController/PackageController.h"
#include "../headers/common/Package.h"
#include "../headers/gameObjects/graphicEnv.h"

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

void drawField(renderer renderer, SDL_Surface* surf2, SDL_Texture* txtr1, SDL_Texture* txtr2);

int main()
{
	SDLInit();
	ClientController clientController;
	clientController.connect("127.0.0.1", 9999);
	int pIndex = -1;
	PackageController packageController{};
	while (!clientController.bothPlayersConnected && pIndex==-1)
	{
		printf("waiting for both players to connect\n");
		packageController.receivePackages(clientController.getServerSocket(),0);
		while (!packageController.receivedPackagesQueue[0].empty())
		{
			Package package = packageController.getFrontReceivedPackage(0);
			if (package.message == infoBothConnected)
			{
				clientController.bothPlayersConnected = package.data[0];
				printf("all connected");
			}
			else if (package.message == setIndex)
			{
				pIndex = package.data[0];
				printf("my index: %d\n", pIndex);
			}
			else
				printf("%d\n",package.message);
		}
	}

	bool done = false;
	SDL_Event event;

	SDL_Window* win = NULL;
	string name = "morskoy.boy " + to_string(pIndex);

	win = SDL_CreateWindow(name.c_str(), 0, 25, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!win)
	{
		std::cerr << "Failed to create Window: " << SDL_GetError() << std::endl;
		return 5;
	}
	renderer renderer(win);

	GameClientController gameClientController(renderer.getRen());
	gameClientController.playerIndexOnServer = pIndex;
	graphicEnv gameUI;

	SDL_Surface* surf1 = NULL;
	SDL_Texture* txtr1 = NULL;
	SDL_Surface* surf2 = NULL;
	SDL_Texture* txtr2 = NULL;

	surf1 = SDL_LoadBMP("res/background.bmp");
	if (!surf1)
	{
		std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
		return false;
	}
	txtr1 = SDL_CreateTextureFromSurface(renderer.getRen(), surf1);
	if (!txtr1)
	{
		std::cerr << "Texture (background) wasn't created: " << SDL_GetError() << std::endl;
		return false;
	}

	surf2 = SDL_LoadBMP("res/fields.bmp");
	if (!surf2)
	{
		std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
		return false;
	}
	txtr2 = SDL_CreateTextureFromSurface(renderer.getRen(), surf2);
	if (!txtr2)
	{
		std::cerr << "Texture (field) wasn't created: " << SDL_GetError() << std::endl;
		return false;
	}
	int timer = 0;
	while (!done)
	{
		drawField(renderer, surf2, txtr1, txtr2);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				done = false;
				break;
			}
			Package pkg = gameClientController.handleMouseEvent(event);
			if (pkg.message != emptyPackage)
			{
				packageController.addPackageToSendQueue(pkg,0);
			}
		}
		
		if (timer++ > 1000)
		{
			packageController.receivePackages(clientController.getServerSocket(), 0);
			while (!packageController.receivedPackagesQueue[0].empty())
			{
				gameClientController.proceedResponse(packageController.receivedPackagesQueue[0].front());
				packageController.receivedPackagesQueue[0].pop();
			}

			Package fieldRequest{ {gameClientController.playerIndexOnServer}, requestField };
			Package msgRequest{ {gameClientController.playerIndexOnServer}, requestMessages};
			packageController.addPackageToSendQueue(fieldRequest, 0);
			packageController.addPackageToSendQueue(msgRequest, 0);
			packageController.sendPackages(clientController.getServerSocket(), 0);
			timer = 0;
		}
		for (int player = 0; player < 2; player++) 
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					switch (gameClientController.playerFields[player][i][j])
					{
					case shipSize1:
					case shipSize2:
					case shipSize3:
					case shipSize4:
					{
						SDL_Rect currentPos = { (player*(SCREEN_WIDTH/2)) + j * 30 + 30, i * 30 + 30, 30,30 };
						SDL_RenderCopy(renderer.getRen(), gameClientController.ships[0][0].getShipTexture(), NULL, &currentPos);
					}
					default:
						break;
					}
				}
			}
		}
		for (int i = 0; i < maxAmount; i++)
		{
			for (int j = 0; j < maxAmount - i; j++)
			{
				gameClientController.ships[i][j].image();
			}
		}

		SDL_RenderPresent(renderer.getRen());
	}
	/*Package request = gameClientController.makeShot(0, 0);
	packageController.addPackageToSendQueue(request);
	packageController.sendPackages(clientController.getServerSocket());*/
}

void drawField(renderer renderer, SDL_Surface* surf2, SDL_Texture* txtr1, SDL_Texture* txtr2)
{
	SDL_Rect dst;
	SDL_RenderCopy(renderer.getRen(), txtr1, NULL, NULL);
	dst.x = scale_x(30);
	dst.y = scale_y(30);
	dst.h = scale_y(surf2->h);
	dst.w = scale_y(surf2->w);
	SDL_RenderCopy(renderer.getRen(), txtr2, NULL, &dst);

	dst.x = scale_x(30) + scale_x(300) + scale_x(60);
	SDL_RenderCopy(renderer.getRen(), txtr2, NULL, &dst);
}