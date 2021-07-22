#include <SDL.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#undef main

#include <iostream>
#include <thread>

#include "../headers/client/ClientController/ClientController.h"
#include "../headers/client/GameClientController/GameClientController.h"
#include "../headers/common/PackageController/PackageController.h"
#include "../headers/common/Package.h"
#include "../headers/gameObjects/graphicEnv.h"
#include "../headers/gameObjects/button/Button.h"

using namespace std;

const int WIDTH = 720, HEIGHT = 540,
buttonsAmount = 3, initialPositionY = HEIGHT / 6, ENDGAME = 7;

int SDLInit() {
	if (SDL_Init(0) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		return 1;
	}
	if (SDLNet_Init() == -1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return 2;
	}
	if (TTF_Init() == -1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		return 3;
	}
	return 0;
}

SDL_Color black = { 0,0,0 };
SDL_Color red = { 255,0,0 };
SDL_Color white = { 255,255,255 };

void drawField(SDL_Renderer* renderer, SDL_Rect surfSize, SDL_Texture* txtr1, SDL_Texture* txtr2);

int start(SDL_Renderer* renderer, TTF_Font* font);

int help(SDL_Renderer* renderer, TTF_Font* font)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	const int lines = 8;
	string strings[lines] =
	{
		"Первый этап игры - расстановка кораблей",
		"Нажмите на корабль левой кнопкой мыши чтобы его взять",
		"Затем нажмите на поле левой кнопкой мыши чтобы его поставить",
		"Для поворота корабля нажмите правую кнопку мыши",
		"После расстановки всех кораблей начнется бой",
		"Во время боя нажмите на клетку на поле врага чтобы выстрелить",
		"При попадании нажмите на другую клетку чтобы выстрелить еще раз",
		"Нажмите escape чтобы вернуться в главное меню"
	};
	for (int i = 0; i < lines; i++)
	{
		drawText(renderer, font, SCREEN_WIDTH / 20, SCREEN_HEIGHT / (lines + 2) + TTF_FontHeight(font) * 2 * i, strings[i].c_str(), white);
	}
	SDL_RenderPresent(renderer);
	SDL_Event event = SDL_Event();
	while (event.key.keysym.sym != SDLK_ESCAPE)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
		{
			SDL_Quit();
			return 0;
		}
	}
	return 1;
}

int menu(SDL_Renderer* renderer, TTF_Font* font)
{
	Button buttons[buttonsAmount];
	buttons[0].setText("1. начать игру");
	buttons[1].setText("2. помощь");
	buttons[2].setText("3. выйти");

	int x, y;
	SDL_Color defaultButtonColor = white;

	SDL_Color buttonColors[buttonsAmount];

	for (int i = 0; i < buttonsAmount; i++)
	{
		buttons[i].setSize(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 15);
		buttons[i].setPosition(SCREEN_WIDTH / 2 - buttons[i].width / 2, initialPositionY + HEIGHT / 8 * i);
		buttonColors[i] = white;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_Event event;
	bool done = false;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			SDL_GetMouseState(&x, &y);
			for (int i = 0; i < buttonsAmount; i++)
			{
				if (positionIsInRect(x, y, buttons[i].getRect()))
				{
					buttonColors[i] = red;
				}
				else
					buttonColors[i] = defaultButtonColor;
			}
			if (event.type == SDL_QUIT)
			{
				SDL_Quit();
				return 0;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_1:
					if (start(renderer, font) == ENDGAME)
						return 1;
					else
						return 0;
					break;
				case SDLK_2:
					if (!help(renderer, font))
						return 0;
					break;
				case SDLK_3:
					SDL_Quit();
					return 0;
				default:
					break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{

				for (int i = 0; i < buttonsAmount; i++)
				{
					if (positionIsInRect(x, y, buttons[i].getRect()))
					{
						switch (i)
						{
						case 0:
							if (start(renderer, font) == ENDGAME)
								return 1;
							else
								return 0;
							break;
						case 1:
							if (!help(renderer, font))
								return 0;
							break;
						case 2:
							SDL_Quit();
							return 0;
						default:
							break;
						}
					}
				}
			}
		}
		for (int i = 0; i < buttonsAmount; i++)
			buttons[i].draw(renderer, buttonColors[i], font, white);

		drawText(renderer, font, 10, 10, "морской бой", white);
		drawText(renderer, font, 10, 10 + TTF_FontHeight(font) + 10, "И594 Алешин Андрей", white);
		SDL_RenderPresent(renderer);
		SDL_Delay(32);
		SDL_RenderClear(renderer);

	}
	return 1;
}

int main()
{
	SDLInit();

	bool done = false;

	SDL_Window* win = NULL;
	string name = "Морской бой";

	TTF_Font* font = TTF_OpenFont("res/roboto.ttf", 20);

	win = SDL_CreateWindow(name.c_str(), 0, 25, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!win)
	{
		std::cerr << "Failed to create Window: " << SDL_GetError() << std::endl;
		return 5;
	}
	renderer renderer(win);

	while (menu(renderer.getRen(), font));
}

int start(SDL_Renderer* renderer, TTF_Font* font)
{
	SDL_RenderClear(renderer);
	drawText(renderer, font, 10, 10, "Соединение с сервером", white);
	SDL_RenderPresent(renderer);
	ClientController clientController;
	clientController.connect("127.0.0.1", 9999);

	int pIndex = -1;
	PackageController packageController{};

	while (!clientController.bothPlayersConnected && pIndex == -1)
	{
		printf("waiting for both players to connect\n");
		packageController.receivePackages(clientController.getServerSocket(), 0);
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
				printf("%d\n", package.message);
		}
	}

	bool done = false;
	SDL_Event event;

	SDL_Window* win = NULL;
	string name = "morskoy.boy " + to_string(pIndex);


	GameClientController gameClientController;
	gameClientController.playerIndexOnServer = pIndex;
	gameClientController.placeShips(renderer);
	graphicEnv gameUI;

	SDL_Surface* surf = NULL;
	SDL_Texture* txtr1 = NULL;
	SDL_Texture* txtr2 = NULL;


	surf = SDL_LoadBMP("res/background.bmp");
	if (!surf)
	{
		std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
		return false;
	}
	txtr1 = SDL_CreateTextureFromSurface(renderer, surf);
	if (!txtr1)
	{
		std::cerr << "Texture (background) wasn't created: " << SDL_GetError() << std::endl;
		return false;
	}


	surf = SDL_LoadBMP("res/fields.bmp");
	if (!surf)
	{
		std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_Rect fieldSize = { 0,0,surf->w,surf->h };

	txtr2 = SDL_CreateTextureFromSurface(renderer, surf);
	if (!txtr2)
	{
		std::cerr << "Texture (field) wasn't created: " << SDL_GetError() << std::endl;
		return false;
	}
	int timer = 0;

	surf = SDL_LoadBMP("res/damaged.bmp");
	SDL_Texture* damagedTexture = SDL_CreateTextureFromSurface(renderer, surf);
	surf = SDL_LoadBMP("res/missed.bmp");
	SDL_Texture* missedTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_Rect currentPos;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				done = true;
				break;
			}

			if (gameClientController.gameState == results)
				continue;

			Package pkg = gameClientController.handleMouseEvent(event);
			if (pkg.message != emptyPackage)
			{
				packageController.addPackageToSendQueue(pkg, 0);
			}
		}

		if (gameClientController.gameState == results)
		{
			SDL_RenderClear(renderer);
			string losewin[2] = { "проиграли", "победили" };
			string result = "Вы " + losewin[gameClientController.gameWinner == gameClientController.playerIndexOnServer];
			drawText(renderer, font, 10, 10, result.c_str(), white);
			SDL_RenderPresent(renderer);
			continue;
		}
		drawField(renderer, fieldSize, txtr1, txtr2);

		if (timer > 1000)
		{
			Package fieldRequest{ {gameClientController.playerIndexOnServer}, requestField };
			Package msgRequest{ {gameClientController.playerIndexOnServer}, requestMessages };
			packageController.addPackageToSendQueue(fieldRequest, 0);
			packageController.addPackageToSendQueue(msgRequest, 0);

			if (gameClientController.gameState == battle)
			{
				Package whosTurnRequest{ {gameClientController.playerIndexOnServer}, requestTurn };
				packageController.addPackageToSendQueue(whosTurnRequest, 0);
			}

			packageController.sendPackages(clientController.getServerSocket(), 0);
		}
		for (int player = 0; player < 2; player++)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					currentPos = { (player * (SCREEN_WIDTH / 2)) + j * 30 + 30, i * 30 + 30, 30,30 };
					switch (gameClientController.playerFields[player][i][j])
					{
					case shipDamaged:
					{
						SDL_RenderCopy(renderer, damagedTexture, NULL, &currentPos);
						break;
					}
					case shotMissed:
					{
						SDL_RenderCopy(renderer, missedTexture, NULL, &currentPos);
						break;
					}
					case shipSize1:
					case shipSize2:
					case shipSize3:
					case shipSize4:
					{
						SDL_RenderCopy(renderer, gameClientController.ships[0][0].getShipTexture(), NULL, &currentPos);
						break;
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

		SDL_RenderPresent(renderer);

		if (timer++ > 1000)
		{
			packageController.receivePackages(clientController.getServerSocket(), 0);
			while (!packageController.receivedPackagesQueue[0].empty())
			{
				Package p = packageController.receivedPackagesQueue[0].front();
				gameClientController.proceedResponse(p);
				packageController.receivedPackagesQueue[0].pop();
			}

			timer = 0;
		}
	}
}

void drawField(SDL_Renderer* renderer, SDL_Rect surfSize, SDL_Texture* txtr1, SDL_Texture* txtr2)
{
	SDL_Rect dst;
	SDL_RenderCopy(renderer, txtr1, NULL, NULL);
	dst.x = scale_x(30);
	dst.y = scale_y(30);
	dst.h = scale_y(surfSize.h);
	dst.w = scale_y(surfSize.w);
	SDL_RenderCopy(renderer, txtr2, NULL, &dst);

	dst.x = scale_x(30) + scale_x(300) + scale_x(60);
	SDL_RenderCopy(renderer, txtr2, NULL, &dst);
}
