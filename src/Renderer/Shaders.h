#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Renderer {
	class Shader {
	public:
		unsigned int ID;

		Shader(const std::string& resoursePath, const char* vertexName, const char* fragmentName, const char* geometryName = nullptr);
		Shader() = delete;
		Shader(Shader* drop) = delete;
		Shader(const Shader* drop) = delete;

		void Use();

		void SetUniform(std::string name, float x, float y, float z) const;
		void SetUniform(std::string name, int data) const;
		void SetUniform(std::string name, unsigned int data) const;
		void SetUniform(std::string name, float data) const;
		void SetUniform(std::string name, glm::vec3& value) const;
		void SetUniform(std::string name, glm::vec2& value) const;
		void SetUniform(std::string name, glm::mat4& data) const;
	private:
		std::string GetShaderCode(std::string resoursePath, const char* shaderName, std::ifstream& shaderFile);
	};
}