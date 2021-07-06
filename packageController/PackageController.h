#ifndef PACKAGECONTROLLERH
#define PACKAGECONTROLLERH
#include <queue>
#include <SDL_net.h>
#include "Package.h"

class PackageController
{
	//bool serverSide;
	std::queue<Package> packageQueue;

	void sendPackage(TCPsocket receiver, Package package);
	Package receivePackage(TCPsocket sender);

};

#endif // !PACKAGECONTROLLERH

