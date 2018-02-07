#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glm.hpp>
#include "shader.h"

struct Vertex
{
	// ����.
	glm::vec3 Position;
	// ������.
	glm::vec3 Normal;
	// ��������.
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
		const std::vector<Texture>& textures);
	void Draw(const Shader& shader);

private:
	unsigned int VAO, VBO, EBO;
	void SetupMesh();

};

#endif // MESH_H