#ifndef GAMECLIENTCONTROLLERH
#define GAMECLIENTCONTROLLERH
#include "Package.h"
class GameClientController
{
	int playerFields[2][10][10];
	int playerIndexOnServer;

	Package makeShot(int x, int y);
	void proceedResponse(Package response);
};

#endif // !GAMECLIENTCONTROLLERH
