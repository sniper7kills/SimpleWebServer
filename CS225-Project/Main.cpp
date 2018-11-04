/*
	TCP Echo server example in winsock
	Live Server on port 8888
*/
#include<stdio.h>
#include "Webserver.h"

int main(int argc, char *argv[])
{
	Webserver server(8888, "C:\\Users\\User\\OneDrive\\School Work\\Fall 2018\\CS 225\\Project\\CS225-Project\\CS225-Project\\HTML_FILES", 64);
	server.init();
	server.run();
}