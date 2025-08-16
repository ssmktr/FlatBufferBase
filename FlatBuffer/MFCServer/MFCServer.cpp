#include "pch.h"
#include "ChatServer.h"

const unsigned short PORT_NUMBER = 31400;

int main()
{
	CChatServer server(PORT_NUMBER);

	server.Start();

	getchar();

	server.Stop();

	return 0;
}