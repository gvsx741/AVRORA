#pragma once

#include <glad/glad.h>

#include <vector>
#include <iostream>
#include <string>

class ResourceManager {
public:
	ResourceManager() = default;
	~ResourceManager();

};

std::string GetResourcePath(const std::string& executablePath);
unsigned int TextureLoad(const std::string& directory, const char* texName, bool gamma = false);
unsigned int loadCubemap(const std::string& directory, std::vector<std::string> faces);
