#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Resources/stb_image.h"

std::string getResourcePath(const std::string& executablePath) {
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

GLenum determineFormat(int Channels) {
	GLenum format;
	if (Channels == 1) { format = GL_RED; }
	else if (Channels == 3) { format = GL_RGB; }
	else if (Channels == 4) { format = GL_RGBA; }

	return format;
}

unsigned int textureLoad(std::string directory, std::string texName, bool gamma) {
	std::string filename = directory + texName;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int nrComponents, width, height;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format = determineFormat(nrComponents);

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

unsigned int loadCubemap(std::string directory, std::vector<std::string> faces) {

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

//for multi-threading loading
texture loadImage(const std::string& path) {
	texture tex;
	tex.data = stbi_load(path.c_str(), &tex.width, &tex.height, &tex.channels, 0);
	return tex;
}

//uploading need textures
void uploadTextures(std::string resPath, std::vector<std::string>& names, std::vector<unsigned int*>& texIds) {

	unsigned int n = names.size();

	std::vector<std::future<texture>> f(n);

	for (int i = 0; i < n; ++i) {
		f[i] = std::async(std::launch::async, loadImage, resPath + names[i]);
	}

	for (int i = 0; i < n; ++i) {

		texture tex;
		f[i].wait();
		tex = f[i].get();

		if (tex.data) {

			glGenTextures(1, texIds[i]);
			glBindTexture(GL_TEXTURE_2D, *texIds[i]);

			GLenum format = determineFormat(tex.channels);

			glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, tex.data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			stbi_image_free(tex.data);
		}
		else {
			std::cout << "Failed to load texture!" << std::endl;
			stbi_image_free(tex.data);
		}
	}
}

ResourceManager::~ResourceManager() {

}