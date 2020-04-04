#include "GameObject.h"

#include "AABB.h"

/*
	GameObject is responsible for handling the rendering and updating of objects in the game world
	The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
*/

GameObject::GameObject(glm::vec3 &entityPosition, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f); // starts out stationary
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	boundingBox = glm::vec2(scale.x, scale.y);
	rotation = 0;
}

GameObject::GameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f); // starts out stationary
	scale = entityScale;
	boundingBox = glm::vec2(scale.x, scale.y);
	rotation = entityRotation;
}

GameObject::GameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = entityVelocity;
	scale = entityScale;
	boundingBox = glm::vec2(scale.x, scale.y);
	rotation = entityRotation;
}

// Updates the GameObject's state
void GameObject::update(std::vector<shared_ptr<GameObject>>& gameObjects, double deltaTime) {
	// Added some code to rotate the straight velocity verctor before adding it to the position
	glm::vec3 rotatedVelocity = glm::vec3(
		cos(glm::radians(angle)) * velocity.x - sin(glm::radians(angle)) * velocity.y,
		sin(glm::radians(angle)) * velocity.x + cos(glm::radians(angle)) * velocity.y,
		0
	);

	// Update object position with Euler integration
	position += rotatedVelocity * (float)deltaTime + 0.5f * acceleration * (float)deltaTime * (float)deltaTime;
}

// Renders the GameObject using the shader
void GameObject::render(Shader& spriteShader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a rotation matrix to rotate the sprite depending on the angle
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0, 0, 1.0f));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	spriteShader.setUniform1i("count", 0);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

// Renders the GameObject using the shader
void GameObject::renderParticles(Shader& particlesShader) {}

void GameObject::renderBoundingBox(Shader& spriteShader) {
	glBindTexture(GL_TEXTURE_2D, boundingBoxTextureID);

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(boundingBox, 1.0f));

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	spriteShader.setUniform1i("count", 0);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void GameObject::clean() {}

GLboolean GameObject::checkCollision(const GameObject& other) {
	glm::vec3 newCurrentPos = position;
	glm::vec3 newOtherPos = other.position;
	// Align the coordinates on the axis to remove any minus coordinates
	glm::vec3 alignment = glm::vec3(
		glm::min(position.x, other.position.x) - glm::max(boundingBox.x, other.boundingBox.x) / 2,
		glm::min(position.y, other.position.y) - glm::max(boundingBox.y, other.boundingBox.y) / 2,
		0.0f
	);

	//If the alignment adjustment is smaller than 0, add the inverse to the position so we can align
	if (alignment.x < 0) {
		newCurrentPos.x += -alignment.x;
		newOtherPos.x += -alignment.x;
	}
	if (alignment.y < 0) {
		newCurrentPos.y += -alignment.y;
		newOtherPos.y += -alignment.y;
	}

	// Create the bounding boxes for the current object and the other object
	AABB::AABB currentBoundingBox(newCurrentPos, newCurrentPos + glm::vec3(boundingBox, 0));
	// Move the bounding box to the center using the scale
	currentBoundingBox.translate(glm::vec3(-boundingBox.x / 2, -boundingBox.y / 2, 0.0f));

	AABB::AABB otherBoundingBox(newOtherPos, newOtherPos + glm::vec3(other.boundingBox, 0));
	// Move the bounding box to the center using the scale
	otherBoundingBox.translate(glm::vec3(-other.boundingBox.x / 2, -other.boundingBox.y / 2, 0.0f));

	return currentBoundingBox.collides(otherBoundingBox);
}

CollisionSides GameObject::getCollisionSide(const GameObject& other) {
	glm::vec3 sideVector = other.position - position;

	// Get the angle of the vector leading from other to current, rotated by 45°
	GLfloat angle = glm::atan(sideVector.x, sideVector.y) - glm::radians(45.0f);

	GLfloat sin = glm::sin(angle);
	GLfloat cos = glm::cos(angle);

	if (sin > 0 && cos < 0) {
		// Left top quadrant, rotated to be top quadrant
		return CollisionSides::Top;
	}
	else if (sin > 0 && cos > 0) {
		// Right top quadrant, rotated to be right quadrant (So current is on the left)
		return CollisionSides::Left;
	}
	else if (sin < 0 && cos > 0) {
		// Right bottom quadrant, rotated to be bottom quadrant
		return CollisionSides::Bottom;
	}
	else if (sin < 0 && cos < 0) {
		// Left bottom quadrant, rotated to be left quadrant (So current is on the right)
		return CollisionSides::Right;
	}

	// In doubt, collide on the bottom
	return CollisionSides::Bottom;
}

GLuint GameObject::boundingBoxTextureID = 0;

int GameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\FX\\bounding-box.png");
	boundingBoxTextureID = textures[offset + 0];
	return offset + 1;
}