#pragma once

#include <string>

#include "Mathematics.h"

namespace graphics
{
	class Shader
	{

	public:
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
		Shader(std::string vertexShader, std::string fragmentShader);
		Shader(int id) : _ID(id) { }
		~Shader();

		/* Activation of shader */
		void Use() const;
		void Unbind() const;

		// Uniform utility functions
		void SetUniform2f(const std::string& name, const math::vec2<float>& value);
		void SetUniform3f(const std::string& name, const math::vec3<float>& value);
		void SetUniform4f(const std::string& name, const math::vec4<float>& value);
		void SetUniformMatrix4f(const std::string& name, const math::mat4<float>& data);

		inline unsigned int ID() const { return _ID; }

	private:
		/* Get location of uniform in fragment shader with given name */
		int GetUniformLocation(const std::string& name);

		std::string LoadShaderFromFile(const std::string& path) const;
		unsigned int CompileShader(unsigned int type, std::string& source) const;
		unsigned int CreateShader(std::string& vertexShader, std::string& fragmenShader) const;

		unsigned int _ID;
	};
}
