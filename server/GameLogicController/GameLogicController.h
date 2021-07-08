#ifndef GAMELOGICCONTROLLERH
#define GAMELOGICCONTROLLERH
#include "Package.h"
class GameLogicController
{
public:
	int playerFields[2][10][10];

	Message proceedShot(int playerIndex, int x, int y);
	Package proceedRequest(Package package);
};

#endif // !GAMELOGICCONTROLLERH


