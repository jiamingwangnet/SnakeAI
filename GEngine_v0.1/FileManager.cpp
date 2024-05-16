#include "FileManager.h"
#include <fstream>
#include <cassert>

bool GEngine::FileManager::ReadToBuffer(std::string path, std::vector<unsigned char>& buffer)
{
	std::ifstream file{ path, std::ios::binary };
	assert(!file.fail());
	if (file.fail())
		return false;

	// get file size
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();

	file.seekg(0, std::ios::beg);
	fileSize -= file.tellg(); // disregard any headers

	buffer.resize(fileSize);
	file.read((char*)buffer.data(), fileSize);
	
	file.close();
	return true;
}

bool GEngine::FileManager::ReadToBuffer(std::string path, std::string& buffer) // exact same code
{
	std::ifstream file{ path, std::ios::binary };
	assert(!file.fail());
	if (file.fail())
		return false;

	// get file size
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();

	file.seekg(0, std::ios::beg);
	fileSize -= file.tellg(); // disregard any headers

	buffer.resize(fileSize);
	file.read((char*)buffer.data(), fileSize);

	file.close();
	return true;
}
