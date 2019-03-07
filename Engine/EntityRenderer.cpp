#include <glad/glad.h>
#include "../Models/RawModel.h"
#include "../Textures/ModelTexture.h"
#include "../Utils/Math.h"
#include <glad/glad.h>
#include "MasterRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h> 
#include "EntityRenderer.h"

EntityRenderer::EntityRenderer(StaticShader* shader, glm::mat4 projectionMatrix) {
	this->shader = shader;
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
}
EntityRenderer::~EntityRenderer() {}

void EntityRenderer::render(std::map<TexturedModel*, std::vector<Entity*>*>* entities) {
	for (std::map<TexturedModel*, std::vector<Entity*>*>::iterator it = entities->begin(); it != entities->end(); ++it) {
		TexturedModel* model = it->first;

		prepareTexturedModel(model);
		std::vector<Entity*>* batch = entities->at(model);
		for (Entity* entity : *batch) {
			prepareInstance(entity);
			glDrawElements(GL_TRIANGLES, model->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		unbindTexturedModel();
	}
}

void EntityRenderer::prepareTexturedModel(TexturedModel* texturedModel) {
	ModelTexture* texture = texturedModel->getTexture();
	RawModel* model = texturedModel->getRawModel();

	// bind vao
	glBindVertexArray(model->getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// Check if texture has transparency
	if (texture->getTransparency())
		MasterRenderer::disableCulling();
	shader->loadFakeLighting(texture->getFakeLighting());

	// Load the reflectivity variables
	shader->loadShineVariables(texture->getShineDamper(), texture->getReflectivity());

	// activate the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturedModel->getTexture()->getID());
}

void EntityRenderer::unbindTexturedModel() {
	MasterRenderer::enableCulling();

	// unbind the vao
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(Entity* entity) {
	// load transformation from the entity
	glm::mat4* transform = Math::createTransformationMatrix(entity->getPos(), entity->getRx(), entity->getRy(), entity->getRz(), entity->getScale());
	shader->loadTransformationMatrix(*transform);
}