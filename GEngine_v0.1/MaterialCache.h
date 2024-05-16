#pragma once

#include <string>
#include <GL/glew.h>
#include "GLTexture.h"
#include <unordered_map>
#include <vector>
#include "Matrix.h"

namespace GEngine
{
	/*
	Support for multiple shaders

	- store shaders by program id similar to how textures are stored
	- shaders are bound to a material
		- update the Sprite class to support materials
		- a material has a image texture and a shader
		- same materials are rendered together
	- shaders are compiled at during the init function
	- shaders are cached using a ShaderCache class and retrieved using the resource manager
	- update the sprite batch (Renderer) to render sprites with the same texture and shader together
	
	*/

	struct Material
	{
		GLTexture texture;
		GLuint programID;

		bool operator==(const Material& rhs) const
		{
			return texture == rhs.texture && programID == rhs.programID;
		}

		bool operator!=(const Material& rhs) const
		{
			return !(*this == rhs);
		}

		void UseShader() const
		{
			glUseProgram(programID);
			for (int i = 0; i < nAttrib; i++)
				glEnableVertexAttribArray(i);
		}

		void UnuseShader() const
		{
			glUseProgram(0);
			for (int i = 0; i < nAttrib; i++)
				glDisableVertexAttribArray(i);
		}

		void AddAttribute(const std::string& name)
		{
			glBindAttribLocation(programID, nAttrib++, name.c_str());
		}

		GLint GetUniformLocation(const std::string& name) const
		{
			GLint loc = glGetUniformLocation(programID, name.c_str());
			if (loc == GL_INVALID_INDEX)
				throw std::exception{ ((std::string)"Uniform" + name + " not found in shader.").c_str() };
			return loc;
		}

		template<typename T>
		void SetUniform(const std::string& name, T t) const = delete; // add more specialisations when needed

		template<>
		void SetUniform<float>(const std::string& name, float f) const
		{
			glUniform1f(GetUniformLocation(name), f);
		}

		template<>
		void SetUniform<int>(const std::string& name, int i) const
		{
			glUniform1i(GetUniformLocation(name), i);
		}

		template<>
		void SetUniform<Matrix4x4>(const std::string& name, Matrix4x4 m4) const
		{
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &(m4[0][0]));
		}
	private:
		int nAttrib = 0;
	};

	class MaterialCache
	{
	public:
		MaterialCache();
		~MaterialCache();
		
		// attributes must be in order
		Material GetMaterial(const std::string& texturePath, const std::string& vertShaderPath, const std::string& fragShaderPath, const std::vector<std::string> attribs = {"vertexPosition", "vertexColor", "vertexUV"});
		static std::string GetKey(const std::string& texturePath, const std::string& vertShaderPath, const std::string& fragShaderPath);
	private:
		void CompileShaders(const std::string& vertShaderPath, const std::string& fragShaderPath, GLuint& progID_out, GLuint& vertID_out, GLuint& fragID_out);
		void LinkShaders(GLuint programID, GLuint vertShaderID, GLuint fragShaderID);
		void CompileShader(const std::string& path, GLuint shaderID);
	private:
		std::unordered_map<std::string, Material> cache; // combine the three paths together for the key
	};
}