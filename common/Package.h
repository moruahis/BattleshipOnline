#ifndef PACKAGEH
#define PACKAGEH
#define fw 10

enum states {
	emptyCell,
	shipSize1,
	shipSize2,
	shipSize3,
	shipSize4, 
	shipDenied,
	shipDamaged,
	shipDestroyed,
	shotMissed
};

enum Message
{
	// both:
	emptyPackage,
	endOfPackages,
	// coming from client:
	requestPlaceShip,
	requestField,
	requestStrikeInfo,
	requestMessages,
	requestIndex,
	// coming from server:
	fieldInfo,
	strikeSuccess,
	strikeMissed,
	setIndex,
	infoBothConnected,
	timeToSendPackages // utility
};

struct Package {
	int data[1024];
	Message message;
};

#endif // !PACKAGEH
