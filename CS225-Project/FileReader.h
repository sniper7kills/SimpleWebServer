#ifndef FILEREADER
#define FILEREADER

#include <string>
#include <fstream>

using namespace std;

class FileReader
{
private:
	string filename;
	int size;
	string content;
	string fileType;
	ifstream fileStream;
	void readFile();
	void setFileType();
protected:
public:
	FileReader(string filename);
	~FileReader();
	int getFileSize();
	string toString();
	string getFileType();
};

#endif // !FILEREADER
