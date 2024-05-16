#pragma once

#include <string>
#include <vector>

namespace GEngine
{
	class FileManager
	{
	public:
		static bool ReadToBuffer(std::string path, std::vector<unsigned char>& buffer);
		static bool ReadToBuffer(std::string path, std::string& buffer);
	};
}