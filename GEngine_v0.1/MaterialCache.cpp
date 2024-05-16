#include "MaterialCache.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include <iostream>

GEngine::MaterialCache::MaterialCache()
{}

GEngine::MaterialCache::~MaterialCache()
{}

GEngine::Material GEngine::MaterialCache::GetMaterial(const std::string & texturePath, const std::string & vertShaderPath, const std::string & fragShaderPath, const std::vector<std::string> attribs)
{
	std::string key = GetKey(texturePath, vertShaderPath, fragShaderPath);
	auto iter = cache.find(key);

	if (iter == cache.end())
	{
		Material material;

		// create the material
		// get the texture
		material.texture = ResourceManager::GetTexture(texturePath);

		// create the shaders
		GLuint vertProgID;
		GLuint fragProgID;

		CompileShaders(vertShaderPath, fragShaderPath, material.programID, vertProgID, fragProgID);

		// add the attributes
		for (const std::string& attrib : attribs)
		{
			material.AddAttribute(attrib);
		}
		// shaders must be linked after adding attributes
		LinkShaders(material.programID, vertProgID, fragProgID);

		cache.insert({ key, material });
		//std::cout << "Material added to cache.\n";

		return material;
	}

	//std::cout << "Material retrieved from cache.\n";
	return iter->second;
}

std::string GEngine::MaterialCache::GetKey(const std::string& texturePath, const std::string& vertShaderPath, const std::string& fragShaderPath)
{
	return texturePath + vertShaderPath + fragShaderPath;
}

void GEngine::MaterialCache::CompileShaders(const std::string& vertShaderPath, const std::string& fragShaderPath, GLuint& progID_out, GLuint& vertID_out, GLuint& fragID_out)
{
	progID_out = glCreateProgram();

	vertID_out = glCreateShader(GL_VERTEX_SHADER);
	if (vertID_out == 0) throw std::exception{ "Vertex shader creation failed." };

	fragID_out = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragID_out == 0) throw std::exception{ "Fragment shader creation failed." };

	CompileShader(vertShaderPath, vertID_out);
	CompileShader(fragShaderPath, fragID_out);
}

void GEngine::MaterialCache::LinkShaders(GLuint programID, GLuint vertShaderID, GLuint fragShaderID)
{
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);

	glLinkProgram(programID);

	// handle errors
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, errorLog.data());

		glDeleteProgram(programID);

		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);

		std::cout << (const char*)errorLog.data() << '\n';
		throw std::exception{ ((std::string)"Shaders failed to link. \n" + (const char*)errorLog.data()).c_str() };

		return;
	}

	glDetachShader(programID, vertShaderID);
	glDetachShader(programID, fragShaderID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);
}

void GEngine::MaterialCache::CompileShader(const std::string& path, GLuint shaderID)
{
	std::string file;
	FileManager::ReadToBuffer(path, file);

	file.erase(std::remove(file.begin(), file.end(), '\r'), file.end());

	const char* fileStr = file.c_str();
	glShaderSource(shaderID, 1, &fileStr, nullptr);

	glCompileShader(shaderID);

	// handle errors
	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog( maxLength );
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, errorLog.data());

		glDeleteShader(shaderID);

		std::cout << (const char*)errorLog.data() << '\n';
		throw std::exception{ ((std::string)"Shader " + path + " failed to compile. \n" + (const char*)errorLog.data()).c_str() };glm::mat4::operator*=
	}
}
