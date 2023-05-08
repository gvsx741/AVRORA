#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Resources/stb_image.h"

unsigned int TextureLoad(const std::string& directory, const char* texName, bool gamma) {

	std::string& filename = directory + texName;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int nrComponents, width, height;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture!" << std::endl;
		stbi_image_free(data);
	}

	return textureID;	
}

unsigned int loadCubemap(const std::string& directory, std::vector<std::string> faces) {

	std::string filename;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {

		filename = directory + faces[i];
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "ERROR: Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

ResourceManager::~ResourceManager()
{

}

std::string GetResourcePath(const std::string& executablePath)
{
	auto found = executablePath.find_last_of("/\\");
	std::string m_path = executablePath.substr(0, found);

	for (int i = 0; i < sizeof(m_path) / sizeof(char); i++) {
		if (m_path[i] == '\\') {
			m_path[i] = '/';
		}
	}
	m_path += "/res/";
	
	return m_path;
}
