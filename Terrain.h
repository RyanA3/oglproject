#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include "vector";


class Terrain {
private:
	int resolution = 8;
	float width = 1.0f;
	glm::vec3 pos = glm::vec3(0.0f);

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VBO, VAO, EBO;

public:
	Terrain();
	Terrain(int resolution, float width);

	void setupMesh();
	void draw(Shader& shader);

};