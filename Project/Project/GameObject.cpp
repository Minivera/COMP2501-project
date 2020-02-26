#include "GameObject.h"

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
	rotation = 0;
}

GameObject::GameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f); // starts out stationary
	scale = entityScale;
	rotation = entityRotation;
}

GameObject::GameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) {
	position = entityPosition;
	texture = entityTexture;
	numElements = entityNumElements;
	velocity = entityVelocity;
	scale = entityScale;
	rotation = entityRotation;
}

// Updates the GameObject's state
void GameObject::update(std::vector<shared_ptr<GameObject>> gameObjects, double deltaTime) {
	// Added some code to rotate the straight velocity verctor before adding it to the position
	glm::vec3 rotatedVelocity = glm::vec3(
		cos(glm::radians(angle)) * velocity.x - sin(glm::radians(angle)) * velocity.y,
		sin(glm::radians(angle)) * velocity.x + cos(glm::radians(angle)) * velocity.y,
		0
	);

	// Added an offset to make the sprite move as a function of time
	// currentOffset = glm::vec2(sin(glfwGetTime()), cos(glfwGetTime()));

	// Update object position with Euler integration
	position += rotatedVelocity * (float)deltaTime;
}

// Renders the GameObject using the shader
void GameObject::render(Shader &shader) {
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
	shader.setUniformMat4("transformationMatrix", transformationMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void GameObject::clean() {}

GLboolean GameObject::checkCollision(const GameObject& other) {
	glm::vec2 size = glm::vec2(baseSize * scale.x, baseSize * scale.y);

	// Collision x-axis?
	bool collisionX = position.x + size.x >= other.position.x && other.position.x + size.x >= position.x;

	// Collision y-axis?
	bool collisionY = position.y + size.y >= other.position.y && other.position.y + size.y >= position.y;

	// Collision only if on both axes
	return collisionX && collisionY;
}

int GameObject::getCollisionSide(const GameObject& other) {
	return position.x + baseSize * scale.x >= other.position.x ? -1 : 1;
}