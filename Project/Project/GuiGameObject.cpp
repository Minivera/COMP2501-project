#include "GuiGameObject.h"

GuiGameObject::GuiGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLuint normalTexture, GLuint hoveredTexture, GLuint clickedTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityScale, 0, normalTexture, entityNumElements) {
	this->hoveredTexture = hoveredTexture;
	this->clickedTexture = clickedTexture;
}

void GuiGameObject::verifyMouseOver(glm::vec2 mousePos) {
	glm::vec2 rectStart = glm::vec2(
		position.x - boundingBox.x / 2,
		position.y + boundingBox.y / 2
	);

	glm::vec2 rectEnd = glm::vec2(
		position.x + boundingBox.x / 2,
		position.y - boundingBox.y / 2
	);

	if (mousePos.x > rectStart.x && mousePos.x < rectEnd.x && mousePos.y < rectStart.y && mousePos.y > rectEnd.y) {
		hovered = true;
	}
}

void GuiGameObject::verifyClick() {
	if (hovered && clickable) {
		clicked = true;

		if (onClickListener != nullptr) {
			onClickListener(*this);
		}
	}
}

void GuiGameObject::render(Shader& spriteShader) {
	GLuint selectedTexture = texture;
	if (hovered) {
		selectedTexture = hoveredTexture;
	}
	if (clicked) {
		selectedTexture = clickedTexture;
	}

	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, selectedTexture);
	spriteShader.enable();
	spriteShader.setAttributes();

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	spriteShader.setUniform1i("count", 0);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void GuiGameObject::clean() {
	clicked = false;
	hovered = false;
}