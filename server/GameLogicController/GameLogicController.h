#ifndef GAMELOGICCONTROLLERH
#define GAMELOGICCONTROLLERH
#include "../../common/Package.h"
class GameLogicController
{
public:
	int playerFields[2][10][10];
	int shipsPlaced[2];
	bool canShoot[2];

	GameLogicController();

	Message proceedShot(int playerIndex, int x, int y);
	Package proceedRequest(Package package);
};

#endif // !GAMELOGICCONTROLLERH
