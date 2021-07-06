#ifndef PACKAGEH
#define PACKAGEH
#define fw 10

enum Message
{
	//coming from client:
	requestField,
	requestStrikeInfo,
	requestMessages,
	//coming from server:
	fieldInfo,
	strikeSuccess,
	strikeFailure,
};

struct Package {
	int data[1024];

	Message message;
	/*int getDataSize()
	{
		switch (message) {
		case requestStrikeInfo:
			return sizeof(int[2]);
		case strikeSuccess:
		case strikeFailure:
		case requestMessages:
		case requestField:
			return sizeof(bool);
		case fieldInfo:
			return sizeof(int[fw*fw]);
		}
	}*/
};

#endif // !PACKAGEH
