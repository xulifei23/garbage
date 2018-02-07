#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <assimp/scene.h>

unsigned int TextureFromFile(const char* path, const std::string& dir,
	bool gamma = false);

class Model
{
public:
	Model(char* path)
	{
		LoadModel(path);
	}

	void Draw(Shader& shader);

private:
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat,
		aiTextureType type, const std::string& typeName);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;

};

#endif //MODEL_H
