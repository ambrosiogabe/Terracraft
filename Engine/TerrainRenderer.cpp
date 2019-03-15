#include "../Utils/Math.h"
#include "TerrainRenderer.h"


TerrainRenderer::~TerrainRenderer() {}
TerrainRenderer::TerrainRenderer(TerrainShader* shader, glm::mat4 projectionMatrix) {
	this->shader = shader;

	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->loadTextureUnits();
	shader->stop();
}

void TerrainRenderer::render(std::vector<Terrain>* terrains) {
	for (auto& terrain : *terrains) {
		prepareTerrain(&terrain);
		loadModelMatrix(&terrain);
		glDrawElements(GL_TRIANGLES, terrain.getModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
		unbindTexturedModel();
	}
}

void TerrainRenderer::prepareTerrain(Terrain* terrain) {
	RawModel* model = terrain->getModel();

	// bind vao
	glBindVertexArray(model->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// bind the texture
	bindTextures(terrain);

	// Load the reflectivity variables
	shader->loadShineVariables(1, 0);
}

void TerrainRenderer::bindTextures(Terrain* terrain) {
	TerrainTexturePack* texturePack = terrain->getTexturePack();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePack->getBackgroundTexture()->getId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturePack->getRTexture()->getId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texturePack->getGTexture()->getId());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texturePack->getBTexture()->getId());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->getBlendMap()->getId());
}

void TerrainRenderer::unbindTexturedModel() {
	// unbind the vao
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void TerrainRenderer::loadModelMatrix(Terrain* terrain) {
	// load transformation from the entity
	glm::mat4 transform = Math::createTransformationMatrix(*(new glm::vec3(terrain->getX(), 0, terrain->getZ())), 0, 0, 0, 1);
	shader->loadTransformationMatrix(transform);
}
