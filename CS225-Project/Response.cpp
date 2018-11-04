#include "Response.h"

Response::Response(int code = 200, string message = "INTERNAL SERVER ERROR", string type = "text/html")
{
	this->code = code;
	this->content = message;
	this->type = type;
	this->setResponse();
}

void Response::setResponse()
{
	string response = "HTTP/1.0 ";
	if (this->code == 200) {
		response += "200 OK\r\n";
	}
	else if (this->code == 404) {
		response += "404 Not Found\r\n";
	}
	else {
		response += "500 Internal Server Error";
	}
	response += "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n";
	response += "Server: Will + Richard (CS225) Simple Web GET Server (V 1.0)\r\n";
	response += "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n";
	response += "Content-Length: ";
	response += to_string(this->content.length() * sizeof(char));
	response += "\r\n";
	response += "Content-Type: ";
	response += this->type;
	response += "\r\n";
	response += "Connection: Closed\r\n\r\n";
	response += this->content;
	this->response = response;
	this->length = response.length() * sizeof(char);
}

string Response::toString()
{
	return this->response;
}

int Response::getLength()
{
	return this->length;
}