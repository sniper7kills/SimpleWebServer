#include "FileReader.h"

FileReader::FileReader(string filename)
{
	this->filename = filename;
	try {
		this->fileStream.open(this->filename.c_str(), fstream::in);
	}
	catch (...) {
		throw "ERROR OPENING FILE...";
	}
	if (this->fileStream.fail()) {
		throw(string("FILE COULD NOT BE OPENED!"));
	}

}
FileReader::~FileReader()
{
	this->fileStream.close();
}

int FileReader::getFileSize()
{
	if (this->size != NULL)
		return this->size;

	this->size = (this->content.length() * sizeof(char));
	return this->size;
}

string FileReader::toString()
{
	if (this->content.length() > 0)
		return this->content;

	readFile();
	return this->content;
}

void FileReader::readFile()
{
	this->content.assign(istreambuf_iterator<char>(this->fileStream),
		istreambuf_iterator<char>());
}

void FileReader::setFileType()
{
	string fileExt = this->filename.substr(this->filename.find_first_of(".") + 1);
	if (fileExt == "html" || fileExt == "htm")
		this->fileType = "text/html";
	if (fileExt == "css")
		this->fileType = "text/css";
	if (fileExt == "js")
		this->fileType = "application/javascript";
}

string FileReader::getFileType()
{
	if (this->fileType.length() > 0)
		return this->fileType;
	this->setFileType();
	return this->fileType;
}