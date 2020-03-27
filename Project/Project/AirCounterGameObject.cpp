#include "AirCounterGameObject.h"

#include <sstream>
#include <iomanip>

#include "TextRenderer.h"

AirCounterGameObject::AirCounterGameObject(float amount, glm::vec3& entityPos, glm::vec3 entityScale, GLuint airTextTexture, GLint entityNumElements) :
	GameObject(entityPos, entityScale, 0, airTextTexture, entityNumElements) {
	this->amount = amount;
}

void AirCounterGameObject::render(Shader& spriteShader) {
	int minutes = 0;
	int seconds = amount;

	while (seconds >= 60) {
		minutes += 1;
		seconds -= 60;
	}

	stringstream builder;
	builder << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

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

	TextRenderer::render(builder.str(), transformationMatrix, spriteShader, numElements);
}