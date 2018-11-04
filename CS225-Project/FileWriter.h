#ifndef FILEWRITER
#define FILEWRITER

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class FileWriter
{
private:
protected:
	string filename;
	ofstream fileStream;
public:
	FileWriter(string filename);
	~FileWriter();
	void write(string output);
};

#endif // !FILEWRITER
