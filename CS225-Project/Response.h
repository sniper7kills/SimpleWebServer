#ifndef RESPONSE
#define RESPONSE

#include <string>
using namespace std;

class Response
{
private:
	int code;
	string response;
	string content;
	string type;
	int length;
protected:
public:
	Response(int code, string content, string type);
	void setResponse();
	string toString();
	int getLength();
};

#endif // !RESPONSE
