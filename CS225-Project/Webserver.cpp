#include <iostream>
#include <string>
#include "Webserver.h"
#include "Request.h"
#include "Response.h"
#include "FileReader.h"
#include <bitset>
#include <stdio.h>

using namespace std;

Webserver::~Webserver()
{
	closesocket(s);
	WSACleanup();
}

Webserver::Webserver(int port, string htmlDir, int maxClients = 64) {
	this->port = port;
	this->htmlDir = htmlDir;
	this->maxClients = maxClients;

	//setup the recieve buffer
	this->buffer = (char*)malloc((this->maxRecieveSize + 1) * sizeof(char));

	//initialize client sockets
	for (int i = 0; i < this->maxClients; i++)
	{
		this->clientSocket[i] = 0;
	}
}

void Webserver::init()
{
	//Start Winsock
	cout << endl << "Initialising Winsock..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Failed. Error Code : " << WSAGetLastError() << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Initialized" << endl;

	//Create a Master Socket
	if ((this->masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "Could not create socket : " << WSAGetLastError() << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Socket created." << endl;

	//setup the server struct
	this->server.sin_family = AF_INET;
	this->server.sin_addr.s_addr = INADDR_ANY;
	this->server.sin_port = htons(this->port);

	//Bind the Master Socket to the Server Struct Settings
	if (bind(this->masterSocket, (struct sockaddr *)&this->server, sizeof(this->server)) == SOCKET_ERROR)
	{
		cout << "Bind failed with error code : " << WSAGetLastError() << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Bind Successfull" << endl;

	//Start Listening for new connections
	listen(this->masterSocket, 3);
	cout << "Listening for incomming connections" << endl;

	this->addrlen = sizeof(struct sockaddr_in);
}

void Webserver::run()
{
	while (TRUE)
	{
		//clear the socket fd set
		FD_ZERO(&this->readfds);

		//add master socket to fd set
		FD_SET(this->masterSocket, &this->readfds);

		//add child sockets to fd set
		for (int i = 0; i < this->maxClients; i++)
		{
			this->s = this->clientSocket[i];
			if (this->s > 0)
			{
				FD_SET(this->s, &this->readfds);
			}
		}

		//wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
		this->activity = select(0, &this->readfds, NULL, NULL, NULL);

		//Error selecting the socket
		if (this->activity == SOCKET_ERROR)
		{
			cout << "select call failed with error code : " << WSAGetLastError() << endl;
			exit(EXIT_FAILURE);
		}

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(this->masterSocket, &this->readfds))
		{
			if ((this->newSocket = accept(this->masterSocket, (struct sockaddr *)&this->address, (int *)&this->addrlen)) < 0)
			{
				cout << "Error in accepting the new connection" << endl;
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			cout << "New Connection: IP: " << inet_ntoa(this->address.sin_addr) << ", Port: " << ntohs(this->address.sin_port) << endl;

			//send new connection greeting message
			//if (send(this->new_socket, message, strlen(message), 0) != strlen(message))
			//{
			//	perror("send failed");
			//}

			//puts("Welcome message sent successfully");

			//add new socket to array of sockets
			for (int i = 0; i < this->maxClients; i++)
			{
				if (this->clientSocket[i] == 0)
				{
					this->clientSocket[i] = this->newSocket;
					cout << "New client added to the list of Client Sockets at Index: " << i << endl;
					break;
				}
			}
		}

		//else its some IO operation on some other socket :)
		for (int i = 0; i < this->maxClients; i++)
		{
			this->s = this->clientSocket[i];
			//if client presend in read sockets             
			if (FD_ISSET(this->s, &this->readfds))
			{
				//get details of the client into address var
				getpeername(this->s, (struct sockaddr*)&this->address, (int*)&this->addrlen);

				//Check for new message from Client
				this->valread = recv(this->s, this->buffer, this->maxRecieveSize, 0);

				//Check if socket has an error
				if (this->valread == SOCKET_ERROR)
				{
					int error_code = WSAGetLastError();
					//Connection was reset
					if (error_code == WSAECONNRESET)
					{
						//Somebody disconnected , print details and close socket
						cout << "Host unexpectedly disconnected. IP: " << inet_ntoa(this->address.sin_addr) << ", Port: " << ntohs(this->address.sin_port) << endl;
						closesocket(this->s);
						this->clientSocket[i] = 0;
					}
					else
					{
						//Recieve Failed.... Disconnect Client
						cout << "Recieve Failed with error code: " << error_code << endl;
						closesocket(this->s);
						this->clientSocket[i] = 0;
					}
				}
				//Client Disconnected
				if (valread == 0)
				{
					//Somebody disconnected , print details ... and Close Socket.
					cout << "Host Disconnected. IP: " << inet_ntoa(this->address.sin_addr) << ", Port: " << ntohs(this->address.sin_port) << endl;
					closesocket(this->s);
					this->clientSocket[i] = 0;
				}
				//Good Request Recieved.... Process Request.
				else
				{

					//add null character, if you want to use with printf/puts or other string handling functions
					try {
						Request request(buffer);
						cout << request.fileRequested() << endl;
						FileReader f(this->htmlDir + request.fileRequested());
						Response r(200, f.toString(), f.getFileType());
						send(s, r.toString().c_str(), r.getLength(), 0);
					}
					catch ( string &error) {
						if (error == "FILE COULD NOT BE OPENED!") {
							Response r(404, "<html><head><title>Error!</title></head><body><h1>File Not Found!</h1></body></html>", "text/html");
							send(s, r.toString().c_str(), r.getLength(), 0);
						}
						else if (error == "INVALID REQUEST") {
							Response r(500, "<html><head><title>Invalid Request Type!</title></head><body><h1>Invalid Request Type!</h1><a href=\"/\">Return Home</a></body></html>", "text/html");
							send(s, r.toString().c_str(), r.getLength(), 0);
						}
						else {
							Response r(500, "<html><head><title>Unknown Error</title></head><body><h1>Unknown Error!</h1></body></html>", "text/html");
							send(s, r.toString().c_str(), r.getLength(), 0);
						}
							
							
						
					}
				}
			}
		}
	}
}