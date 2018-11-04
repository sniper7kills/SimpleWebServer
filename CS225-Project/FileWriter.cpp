#include "FileWriter.h"

FileWriter::FileWriter(string filename)
{
	this->filename = filename;
	this->fileStream.open(this->filename.c_str(), fstream::out | fstream::app);
}

FileWriter::~FileWriter()
{
	this->fileStream.close();
}

void FileWriter::write(string output)
{
	this->fileStream << output << endl;
}