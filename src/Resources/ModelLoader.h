#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Renderer/Mesh.h"
#include "../Renderer/ResourceManager.h"
#include "stb_image.h"

#include <vector>
#include <string>

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Renderer::Mesh> meshes;
	std::string directory;

	Model(const std::string& resPath, const char* path);
	~Model() = default;

	void Draw(Renderer::Shader &shader);
	void loadModel(std::string const& path);

private:

	void processNode(aiNode* node, const aiScene* scene);
	Renderer::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
