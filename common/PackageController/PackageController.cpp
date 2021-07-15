#include "PackageController.h"

void PackageController::addPackageToSendQueue(Package package, int index)
{
	sendPackagesQueue[index].push(package);
}

void PackageController::addPackageToReceivedQueue(Package package, int index)
{
	receivedPackagesQueue[index].push(package);
}

void PackageController::sendPackages(TCPsocket receiver, int index)
{
	Package package{ {0}, endOfPackages };
	addPackageToSendQueue(package, index);

	while (!sendPackagesQueue[index].empty())
	{
		package = sendPackagesQueue[index].front();
		sendPackagesQueue[index].pop();

		printf("Sending package: %d\n", package.message);
		int result = SDLNet_TCP_Send(receiver, &package, sizeof(Package));
		if (result < sizeof(package))
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		if (package.message == endOfPackages)
			printf("end sent\n");
	}
}

void PackageController::receivePackages(TCPsocket sender, int index)
{
	Package pkg{ {0}, emptyPackage };
	int failCounter = 0;
	SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(set, sender);
	int numready = SDLNet_CheckSockets(set, 100);
	if (numready == -1) {
		printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
		//most of the time this is a system error, where perror might help you.
		perror("SDLNet_CheckSockets");
	}
	else if (numready) {
		printf("There are %d sockets with activity!\n", numready);
		// check all sockets with SDLNet_SocketReady and handle the active ones.
		if (SDLNet_SocketReady(sender)) {
			printf("Ready\n");
			while (pkg.message != endOfPackages && failCounter < 10)
			{
				int len = SDLNet_TCP_Recv(sender, &pkg, sizeof(Package));
				if (!len) {
					printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
				}
				if (pkg.message == emptyPackage)
				{
					failCounter += 1;
				}
				printf("received: %d\n");
				addPackageToReceivedQueue(pkg, index);
			}
		}
		else
			printf("Not ready\n");
	}
}

Package PackageController::getFrontReceivedPackage(int index)
{
	Package pkg = receivedPackagesQueue[index].front();
	receivedPackagesQueue[index].pop();
	return pkg;
}
