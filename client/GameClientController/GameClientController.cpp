#include "GameClientController.h"
#include <cstring>



int scale_x(double value)
{
	return round((value / 7.2) * ((double)SCREEN_WIDTH / 100));
}
int scale_y(double value)
{
	return round((value / 5.4) * ((double)SCREEN_HEIGHT / 100));
}

GameClientController::GameClientController()
{
	gameWinner = -1;
	gameState = shipPlacement;
	grabbedShip = NULL;
}

void GameClientController::placeShips(SDL_Renderer* ren)
{
	for (int i = 0, startCoord = 360, width = 30; i < maxAmount; i++, width += 30)
	{
		dst[i] = { (playerIndexOnServer * (SCREEN_WIDTH / 2)) + 30, startCoord, width, 30 };
		startCoord += 40;
	}
	for (int i = 0; i < maxAmount; i++)
		ships[i].resize(maxAmount - i);

	//Setting start places for ships
	for (int i = 0; i < maxAmount; i++)
	{
		for (int j = 0; (unsigned long int)j < ships[i].size(); j++)
		{
			ships[i][j].setPlace(dst[i]);
			ships[i][j].setCellsValue(i + 1);
			dst[i].x += scale_x(dst[i].w + 10);
		}
	}

	for (int i = 0; i < maxAmount; i++)
	{
		for (int j = 0; (unsigned long int)j < ships[i].size(); j++)
		{
			if (!ships[i][j].loadSkin(ren))
			{
				std::cerr << "Ship with " << i + 1 << " cells wasn't loaded" << std::endl;
				return;
			}
			dst[i].y += scale_y(50);
			ships[i][j].image();
		}
	}
}

Package GameClientController::createShotPackage(int x, int y)
{
	Package package{ {playerIndexOnServer,x,y}, requestStrikeInfo };
	return package;
}

void GameClientController::proceedResponse(Package response)
{
	int playerIndex, x, y;
	switch (response.message)
	{
	case fieldInfo:
		for (int pl = 0; pl < 2; pl++)
			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 10; j++)
					playerFields[pl][j][i] = response.data[pl * 100 + 10 * j + i];
		break;
	case turnInfo:
		canShoot = response.data[playerIndexOnServer];
		break;
	case strikeSuccess:
		playerIndex = response.data[0];
		x = response.data[1];
		y = response.data[2];
		playerFields[playerIndex][x][y] = shipDamaged;
		if (playerIndex == this->playerIndexOnServer)
		{
			std::cout << "Can shoot now\n";
			canShoot = true;
		}
		break;
	case strikeMissed:
		playerIndex = response.data[0];
		x = response.data[1];
		y = response.data[2];
		playerFields[playerIndex][x][y] = shotMissed;
		break;
	case setIndex:
		playerIndexOnServer = response.data[0];
		break;
	case battleBegins:
		gameState = battle;
		break;
	case battleEnds:
		gameState = results;
		gameWinner = response.data[0];
		break;
	default:
		break;
	}
}

Package GameClientController::handleMouseEvent(SDL_Event _event)
{
	SDL_Event event = _event;

	SDL_Rect mousePos = { event.motion.x, event.motion.y, 1, 1 };

	if (gameState == 0)
	{
		if (grabbedShip == NULL && event.type == SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT)
		{
			for (int i = 0; i < maxAmount; i++)
			{
				for (int j = 0; j < maxAmount - i; j++)
				{
					SDL_Rect currentShipRect = ships[i][j].getPlace();
					if (SDL_HasIntersection(&mousePos, &currentShipRect))
					{
						grabbedShip = &ships[i][j];
						grabbedShip->saveInitialPos();
					}
				}
			}
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			if (grabbedShip != NULL)
			{
				grabbedShip->setPlace(mousePos);
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				int cellX = ((mousePos.x - (playerIndexOnServer * (SCREEN_WIDTH / 2)) - 30) / 30);
				int cellY = (mousePos.y - 30) / 30;


				int shipLength = grabbedShip->getRotation() ? 0 : grabbedShip->getCellsValue() - 1;
				int shipHeight = grabbedShip->getRotation() ? grabbedShip->getCellsValue() - 1 : 0;

				if (cellX > 9 || cellX + shipLength > 9 ||
					cellY > 9 || cellY + shipHeight > 9 ||
					cellX < 0 || cellY < 0)
				{
					grabbedShip->resetPosition();
					grabbedShip = NULL;
					return Package();
				}

				// Check if it's allowed to place the ship
				int cell1X, cell2X;
				int cell1Y, cell2Y;

				cell1X = cellX == 0 ? 0 : cellX - 1;
				cell1Y = cellY == 0 ? 0 : cellY - 1;

				cell2X = cellX + shipLength + 1;
				if (cell2X > 9)
					cell2X = 9;

				cell2Y = cellY + shipHeight + 1;
				if (cell2Y > 9)
					cell2Y = 9;

				for (int i = cellY; i <= cellY + shipHeight; i++)
				{
					for (int j = cellX; j <= cellX + shipLength; j++)
					{
						if (playerFields[playerIndexOnServer][i][j] != emptyCell)
						{
							grabbedShip->resetPosition();
							grabbedShip = NULL;
							return Package();
						}
					}
				}

				for (int i = cell1Y; i <= cell2Y; i++)
				{
					for (int j = cell1X; j <= cell2X; j++)
					{
						playerFields[playerIndexOnServer][i][j] = shipDenied;
					}
				}

				for (int i = cellY; i <= cellY + shipHeight; i++)
				{
					for (int j = cellX; j <= cellX + shipLength; j++)
					{
						playerFields[playerIndexOnServer][i][j] = grabbedShip->getCellsValue();
					}
				}

				grabbedShip->setCell(cellX, cellY);

				Package package
				{
					{
						playerIndexOnServer,
						cellX,
						cellY,
						cell1X,
						cell1Y,
						cell2X,
						cell2Y,
						grabbedShip->getCellsValue(),
						grabbedShip->getRotation()
					},
					requestPlaceShip
				};
				grabbedShip->setPlace(SDL_Rect{ -100,-100,0,0 });
				grabbedShip = NULL;

				return package;
			}
			else if (grabbedShip!=NULL && event.button.button == SDL_BUTTON_RIGHT)
			{
				grabbedShip->rotate();
			}
		}
	}
	else if (gameState == 1)
	{
		int cellX = ((mousePos.x - ((playerIndexOnServer == 0) * (SCREEN_WIDTH / 2)) - 30) / 30);
		int cellY = (mousePos.y - 30) / 30;

		if (event.type == SDL_MOUSEBUTTONDOWN && canShoot)
		{
			if (cellX < 0 || cellX > 9 || cellY < 0 || cellY > 9)
			{
				return Package();
			}
			std::cout << "Shot fired!\n";
			canShoot = false;
			return createShotPackage(cellX, cellY);
		}
	}

	return Package();
}
