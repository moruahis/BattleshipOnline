#include "GameLogicController.h"

Message GameLogicController::proceedShot(int playerIndex, int x, int y)
{
	switch (playerFields[playerIndex][x][y])
	{
	case emptyCell:
		return strikeMissed;
	case shipSize1:
	case shipSize2:
	case shipSize3:
	case shipSize4:
		return strikeSuccess;
	case shipDamaged: // impossible?
	case shipDestroyed: // impossible?
	case shotMissed: // impossible?
	default:
		return emptyPackage;
	}
}

Package GameLogicController::proceedRequest(Package package)
{
	switch (package.message)
	{
	case requestPlaceShip:
		int playerIndex = package.data[0];
		int x = package.data[1];
		int y = package.data[2];
		int shipSize = package.data[3];
		int shipOrientation = package.data[4]; // 0 horizontal, 1 vertical
		for (int i = 0; i < shipSize; i++)
		{
			playerFields[playerIndex]
				[x + (shipOrientation==0) * i]
				[y + (shipOrientation==1) * i] = (shipSize1 + shipSize-1); 
				// to shift in enum
		}
		return Package{};
	case requestField:
		int playerIndex = package.data[0];
		Package package{};
		package.message = fieldInfo;
		package.data[0] = playerIndex;
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				package.data[1 + 10 * i + j] = playerFields[playerIndex][i][j];

		return package;
	case requestStrikeInfo:
		int playerIndex = package.data[0]; // package from player
		int x = package.data[1];
		int y = package.data[2];
		int enemyIndex = playerIndex==0; // get another index

		Package package{ {enemyIndex, x,y}, emptyPackage }; // return info about enemy cell
		package.message = proceedShot(enemyIndex, x, y); // proceed enemy cell
		return package;
	case requestMessages:
		Package package{ {0}, timeToSendPackages };
		return package;
	default:
		break;
	}
	return;
}
