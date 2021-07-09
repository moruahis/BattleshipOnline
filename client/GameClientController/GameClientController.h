#ifndef GAMECLIENTCONTROLLERH
#define GAMECLIENTCONTROLLERH
#include "../../common/Package.h"
class GameClientController
{
public:
	int playerFields[2][10][10];
	int playerIndexOnServer;

	Package makeShot(int x, int y);
	void proceedResponse(Package response);
};

#endif // !GAMECLIENTCONTROLLERH
