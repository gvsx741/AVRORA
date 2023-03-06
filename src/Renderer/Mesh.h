#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/Shaders.h"

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

namespace Renderer {
	class Mesh {
	public:
		unsigned int VAO;

		Mesh() = delete;
		
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Renderer::Shader shader);
		~Mesh() = default;

	private:
		unsigned int VBO, EBO;

		void setupMesh();
	};
}