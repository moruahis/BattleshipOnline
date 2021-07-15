#ifndef PACKAGECONTROLLERH
#define PACKAGECONTROLLERH
#include <queue>
#include <SDL_net.h>
#include "../Package.h"

class PackageController
{
public:
	//bool serverSide;
	std::queue<Package> sendPackagesQueue[2], receivedPackagesQueue[2];
	void addPackageToSendQueue(Package package, int index);
	void addPackageToReceivedQueue(Package package, int index);
	void sendPackages(TCPsocket receiver, int index);
	void receivePackages(TCPsocket sender, int index);
	Package getFrontReceivedPackage(int index);
};

#endif // !PACKAGECONTROLLERH
