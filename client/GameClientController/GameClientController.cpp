#include "GameClientController.h"
#include <cstring>
Package GameClientController::makeShot(int x, int y)
{
	Package package{ {playerIndexOnServer,x,y}, requestStrikeInfo };
	return package;
}

void GameClientController::proceedResponse(Package response)
{
	switch (response.message)
	{
	case fieldInfo:
		int playerIndex = response.data[0];
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				playerFields[playerIndex][i][j] = response.data[1+10*i+j];
		break;
	case strikeSuccess:
		int playerIndex = response.data[0];
		int x = response.data[1];
		int y = response.data[2];
		playerFields[playerIndex][x][y] = shipDamaged;
		break;
	case strikeMissed:
		int playerIndex = response.data[0];
		int x = response.data[1];
		int y = response.data[2];
		playerFields[playerIndex][x][y] = shotMissed;
		break;
	case setIndex:
		playerIndexOnServer = response.data[0];
		break;
	default:
		break;
	}
}