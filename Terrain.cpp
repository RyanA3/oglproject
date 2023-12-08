#include "Terrain.h"

//https://www.youtube.com/watch?v=5zlfJW2VGLM

Terrain::Terrain() {
	//Generate vertices
	for (int row = 0; row < resolution; row++) {
		for (int col = 0; col < resolution; col++) {
			vertices.push_back(0.0f);
			//float x = (((row - ((float)resolution / 2)) / resolution)) * 5;
			//float z = (((col - ((float)resolution / 2)) / resolution)) * 5;
			//vertices.push_back(x * x * z);
		}
	}

	int rows = resolution;
	int cols = resolution;

	for (int row = 0; row < rows-1; row++) {

		indices.push_back(row * cols);

		for (int col = 0; col < cols; col++) {

			indices.push_back((row * cols) + col);
			indices.push_back(((row + 1) * cols) + col);

		}

		indices.push_back((row + 1) * cols + (cols - 1));

	}	

	setupMesh();

	texture = LoadTextureFromFile("C:\\Users\\Ryan\\source\\repos\\oglproject\\oglproject\\resources\\images\\container.jpg", "jpg");
}

Terrain::Terrain(int resolution, float width) : resolution(resolution), width(width) {
	for (int x = 0; x < resolution; x++) {
		for (int z = 0; z < resolution; z++) {
			//vertices.push_back(glm::vec3(x, 0.0f, z));
		}
	}

	setupMesh();
}

void Terrain::setupMesh() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Load vertices into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//Load indices into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Format the VAO
	// - location data (height) -- indices provide x z data
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind the VAO
	glBindVertexArray(0);
}

void Terrain::draw(Shader& shader) {

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("terrainTexture", 0);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	if (shouldUpdateModel) {
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, pos);
		shader.setMat4("model", model);
		shouldUpdateModel = false;
	}

	if (shouldUpdateFeatures) {
		shader.setInt("resolution", resolution);
		shader.setFloat("width", width);
		shader.setFloat("invResolution", 1.0f / resolution);
		shader.setFloat("invResolutionSquare", 1.0f / (resolution * resolution));
		shader.setFloat("ds", ds);
		shader.setFloat("dss", dss);
		shouldUpdateFeatures = false;
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}