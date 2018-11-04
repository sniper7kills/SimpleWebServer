#ifndef REQUEST
#define REQUEST

#include <string>

using namespace std;

class Request
{
private:
	string request;
	string method;
protected:
public:
	Request(char*);
	string fileRequested();
};

#endif