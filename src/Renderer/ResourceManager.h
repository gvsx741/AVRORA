#pragma once

#include <glad/glad.h>
#include <future>
#include <vector>
#include <iostream>
#include <string>

class ResourceManager {
public:
	ResourceManager() = default;
	~ResourceManager();
};

struct texture {
	int width, height, channels;
	unsigned char* data;
};

std::string getResourcePath(const std::string& executablePath);
unsigned int textureLoad(std::string directory, std::string texName, bool gamma = false);
unsigned int loadCubemap(std::string directory, std::vector<std::string> faces);
void uploadTextures(std::string resPath, std::vector<std::string>& names, std::vector<unsigned int*>& texIds);
