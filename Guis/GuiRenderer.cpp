#include "../Utils/Math.h"
#include <glm/mat4x4.hpp>
#include "GuiRenderer.h"


GuiRenderer::~GuiRenderer() {}
GuiRenderer::GuiRenderer(Loader* loader) {
	float positions[] = {-1, 1, -1, -1, 1, 1, 1, -1};
	quad = loader->loadToVao(positions, 8);
	shader = new GuiShader();
}

void GuiRenderer::render(std::vector<GuiTexture*> guis) {
	shader->start();

	// bind vertices
	glBindVertexArray(quad->getVaoID());
	glEnableVertexAttribArray(0);

	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// render
	for (GuiTexture* gui : guis) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui->getTexture());

		glm::mat4 matrix = Math::createTransformationMatrix(gui->getPosition(), gui->getScale());
		shader->loadTransformationMatrix(matrix);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
	}

	// disable transparency (blending)
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// unbind vertices
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	shader->stop();
}

void GuiRenderer::cleanUp() {
	shader->cleanUp();
}