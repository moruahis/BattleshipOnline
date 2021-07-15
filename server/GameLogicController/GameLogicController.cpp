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
	int playerIndex, x, y;
	Package response{};
	switch (package.message)
	{
	case requestPlaceShip: {
		playerIndex = package.data[0];
		x = package.data[1];
		y = package.data[2];
		int shipSize = package.data[3];
		int shipOrientation = package.data[4]; // 0 horizontal, 1 vertical
		for (int i = 0; i < shipSize; i++)
		{
			playerFields[playerIndex]
				[y + (shipOrientation == 1) * i]
			[x + (shipOrientation == 0) * i] = (shipSize1 + shipSize - 1);
			// to shift in enum
		}
		return Package{};
	}
	case requestField: {
		response.message = fieldInfo;
		for (int pl = 0; pl < 2; pl++)
			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 10; j++)
					response.data[pl * 100 + 10 * j + i] = playerFields[pl][j][i];

		return response;
	}
	case requestStrikeInfo:
	{
		playerIndex = package.data[0]; // package from player
		x = response.data[1];
		y = response.data[2];
		int enemyIndex = playerIndex == 0; // get another index

		response = { {enemyIndex, x,y}, proceedShot(enemyIndex, x, y) }; // proceed and return info about enemy cell
		return response;
	}
	case requestMessages:
	{
		response = { {0}, timeToSendPackages };
		return response;
	}
	default:
		break;
	}
	return Package{};
}
