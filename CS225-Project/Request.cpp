#include <iostream>
#include <string>
#include "Request.h"

Request::Request(char* request)
{
	this->request = request;
	this->method = this->request.substr(0, this->request.find(" "));
}

string Request::fileRequested()
{
	if (this->method != "GET") {
		throw(string("INVALID REQUEST"));
	}
	string fileRequestLine = this->request.substr(0, this->request.find('\n'));
	string fileRequest = fileRequestLine.substr(
		(fileRequestLine.find("/")),
		(fileRequestLine.find("HTTP") - 5)
	);
	//If request ends in a forward slash, or isn't a file name, assume it is a file, and append index.html to the request
	if (fileRequest.substr((fileRequest.length() - 1)) == "/") {
		fileRequest += "index.html";
	}
	else if (fileRequest.substr((fileRequest.length() - 5)) != ".html"
		&& fileRequest.substr((fileRequest.length() - 4)) != ".htm"
		&& fileRequest.substr((fileRequest.length() - 4)) != ".css"
		&& fileRequest.substr((fileRequest.length() - 3)) != ".js") {
		fileRequest += "/index.html";
	}
	return fileRequest;
}