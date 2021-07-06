#include "PackageController.h"

void PackageController::sendPackage(TCPsocket receiver, Package package)
{
	/*const int packageSize = 1024 + sizeof(Message);
	char bytes[packageSize];
	memcpy(bytes, &package, packageSize);*/
	int result = SDLNet_TCP_Send(receiver, &package, sizeof(Package));
	if (result < sizeof(package))
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
}

Package PackageController::receivePackage(TCPsocket sender)
{
	Package pkg;
	int len = SDLNet_TCP_Recv(sender, &pkg, sizeof(Package));
	if (!len) {
		printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
	}
	return pkg;
}
