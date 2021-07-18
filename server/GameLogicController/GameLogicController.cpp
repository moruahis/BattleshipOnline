#include "GameLogicController.h"

GameLogicController::GameLogicController()
{
	shipsPlaced[0] = shipsPlaced[1] = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 10; k++)
				playerFields[i][j][k] = 0;
}

Message GameLogicController::proceedShot(int playerIndex, int x, int y)
{
	switch (playerFields[playerIndex][y][x])
	{
	case emptyCell:
	case shipDenied:
	{
		this->playerFields[playerIndex][y][x] = shotMissed;
		return strikeMissed;
	}
	case shipSize1:
	case shipSize2:
	case shipSize3:
	case shipSize4:
	{
		this->playerFields[playerIndex][y][x] = shipDamaged;
		return strikeSuccess;
	}
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
	case requestPlaceShip:
	{
		playerIndex = package.data[0];
		x = package.data[1];
		y = package.data[2];
		int cell1X, cell2X, cell1Y, cell2Y;
		cell1X = package.data[3];
		cell1Y = package.data[4];
		cell2X = package.data[5];
		cell2Y = package.data[6];
		int shipSize = package.data[7];
		int shipOrientation = package.data[8]; // 0 horizontal, 1 vertical
		for (int i = cell1Y; i <= cell2Y; i++)
		{
			for (int j = cell1X; j <= cell2X; j++)
			{
				playerFields[playerIndex][i][j] = shipDenied;
			}
		}
		for (int i = 0; i < shipSize; i++)
		{
			playerFields[playerIndex]
				[y + (shipOrientation == 1) * i]
			[x + (shipOrientation == 0) * i] = (shipSize1 + shipSize - 1);
			// to shift in enum
		}
		shipsPlaced[playerIndex] += 1;
		return Package{};
	}
	case requestField: {
		response.message = fieldInfo;
		for (int pl = 0; pl < 2; pl++)
			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 10; j++)
				{
					if (pl != package.data[0] && ((playerFields[pl][j][i] >= shipSize1 && playerFields[pl][j][i] <= shipDenied)))
						response.data[pl * 100 + 10 * j + i] = emptyCell;
					else
						response.data[pl * 100 + 10 * j + i] = playerFields[pl][j][i];
				}
		return response;
	}
	case requestStrikeInfo:
	{
		playerIndex = package.data[0]; // package from player
		x = package.data[1];
		y = package.data[2];
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
