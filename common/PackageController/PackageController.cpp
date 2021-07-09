#include "PackageController.h"

void PackageController::addPackageToSendQueue(Package package)
{
	sendPackagesQueue.push(package);
}

void PackageController::addPackageToReceivedQueue(Package package)
{
	receivedPackagesQueue.push(package);
}

void PackageController::sendPackages(TCPsocket receiver)
{
	Package package{ {0}, endOfPackages };
	addPackageToSendQueue(package);

	while (!sendPackagesQueue.empty())
	{
		package = sendPackagesQueue.front();
		sendPackagesQueue.pop();
		int result = SDLNet_TCP_Send(receiver, &package, sizeof(Package));
		if (result < sizeof(package))
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		if (package.message == endOfPackages)
			printf("end sent\n");
	}
}

void PackageController::receivePackages(TCPsocket sender)
{
	Package pkg{ {0}, emptyPackage };
	int emptyCounter=0;

	while (pkg.message != endOfPackages && emptyCounter<1000)
	{
		int len = SDLNet_TCP_Recv(sender, &pkg, sizeof(Package));
		if (!len) {
			printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		}
		if (pkg.message == emptyPackage)
		{
			emptyCounter += 1;
			continue;
		}
		addPackageToReceivedQueue(pkg);
	}
}

Package PackageController::getFrontReceivedPackage()
{
	Package pkg = receivedPackagesQueue.front();
	receivedPackagesQueue.pop();
	return pkg;
}
