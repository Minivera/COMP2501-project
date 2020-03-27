#include "TreasureCounterGameObject.h"

#include "TextRenderer.h"

TreasureCounterGameObject::TreasureCounterGameObject(int amount, glm::vec3& entityPos, glm::vec3 entityScale, GLuint treasureTexture, GLint entityNumElements) :
	GameObject(entityPos, entityScale, 0, treasureTexture, entityNumElements) {
	this->amount = amount;
}

void TreasureCounterGameObject::render(Shader& spriteShader) {
	string treasureString = to_string(amount);

	// Render the treasure, then the text centered
	spriteShader.enable();
	spriteShader.setAttributes();

	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the world matrix for both our rendered object
	glm::mat4 worldMatrix = glm::translate(glm::mat4(), position);

	// Added a scale matrix to scale the sprite so it can be rescaled
	worldMatrix = glm::scale(worldMatrix, scale);

	// Create a transformation matrix for the treasure icon.
	glm::mat4 transformationMatrix = glm::translate(worldMatrix, glm::vec3(-0.3f, 0.0f, 0.0f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.3f, 0.3f, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	spriteShader.setUniform1i("count", 0);

	// Create a transformation matrix for the treasure amount.
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	// Move the text to the position
	transformationMatrix = glm::translate(worldMatrix, glm::vec3(0.3f, 0.0f, 0.0f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.15f, 0.15f, 1.0f));

	TextRenderer::render(treasureString, transformationMatrix, spriteShader, numElements);
}