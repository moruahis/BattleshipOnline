#ifndef GAMECLIENTCONTROLLERH
#define GAMECLIENTCONTROLLERH
#include "../../common/Package.h"

#include "../../gameObjects/gameObj.h"
#include <SDL.h>
#include <vector>

#define maxAmount 4
#define SCREEN_WIDTH 720 //1% = 7.2
#define SCREEN_HEIGHT 580 //1% = 5.4

enum currentGameStates {
	shipPlacement,
	battle,
	results
};

class GameClientController
{
public:
	int playerFields[2][10][10];
	int playerIndexOnServer;
	int gameState;
	bool canShoot;
	int gameWinner;

	ship* grabbedShip;
	std::vector <ship> ships[maxAmount];

	SDL_Rect dst[maxAmount];

	GameClientController();
	void placeShips(SDL_Renderer* ren);
	Package createShotPackage(int x, int y);
	void proceedResponse(Package response);
	Package handleMouseEvent(SDL_Event event);
};
int scale_x(double);
int scale_y(double);
#endif // !GAMECLIENTCONTROLLERH
