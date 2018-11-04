#include <iostream>
#include <string>
#include "Webserver.h"

using namespace std;

int main(int argc, char *argv[])
{
	//Need to verify argument passing works properly
	int port,maxClients;
	string htmlDir;
	if(argc < 3){
		cout << "USAGE: server.exe <port> <html_dir> <max_clients>";
		exit(0);
	}
	port = argv[1];
	htmlDir = argv[2];
	if(argc < 4){
		maxClients=64;
	}else{
		maxClients = argv[3];
	}
	
	Webserver server(port, htmlDir, maxClients);
	server.init();
	server.run();
	
	return 0;
}
