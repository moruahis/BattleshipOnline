#ifndef PACKAGECONTROLLERH
#define PACKAGECONTROLLERH
#include <queue>
#include <SDL_net.h>
#include "Package.h"

class PackageController
{
public:
	//bool serverSide;
	std::queue<Package> sendPackagesQueue, receivedPackagesQueue;
	void addPackageToSendQueue(Package package);
	void addPackageToReceivedQueue(Package package);
	void sendPackages(TCPsocket receiver);
	void receivePackages(TCPsocket sender);
	Package getFrontReceivedPackage();
};	

#endif // !PACKAGECONTROLLERH

