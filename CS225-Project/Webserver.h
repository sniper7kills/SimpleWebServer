#ifndef WEBSERVER
#define WEBSERVER

#include <string>

//Socket Stuff Here
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib") //Winsock Library

using namespace std;

class Webserver {
private:
	int port, maxRecieveSize = 1024, maxClients, activity, addrlen, i, valread;
	string htmlDir;
	WSADATA wsa;
	SOCKET masterSocket, newSocket, clientSocket[64], s;
	fd_set readfds;
	struct sockaddr_in server, address;
	char* buffer;
protected:
public:
	Webserver(int port, string htmlDir, int maxClients);
	~Webserver();
	void init();
	void run();
};

#endif // !WEBSERVER
