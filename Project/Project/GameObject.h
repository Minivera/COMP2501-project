#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "Shader.h"

using namespace std;

class GameObject {
protected:
	GameObject(glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);
	GameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
	GameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
public:
	// Updates the GameObject's state. Can be overriden for children
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Renders the GameObject using a shader
	virtual void render(Shader &shader);

	// Cleans the object after an update has occured
	virtual void clean();

	// Getters
	inline glm::vec3& getPosition() { return position; }
	inline glm::vec3& getVelocity() { return velocity; }
	inline GLfloat getAngle() { return angle; }
	inline bool isDirty() { return dirty; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setVelocity(glm::vec3& newVelocity) { velocity = newVelocity; }
	inline void setAngle(GLfloat newAngle) { angle = newAngle; }
	inline void setRotation(GLfloat newRotation) { rotation = newRotation; }
	inline void setDirty(bool newDirty) { dirty = newDirty; }
protected:
	// Defines the base size of all objects in the game (Default to 0.5f for all textures)
	const GLfloat baseSize = 0.5f;

	// Define if the object is dirty and should be deleted
	bool dirty = false;

	// Object's Transform Variables
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	GLfloat rotation = 0.0f;

	// Added an angle for rotatting the object and the velocity
	GLfloat angle = 0.0;

	// Object's details
	GLint numElements;

	// Object's texture
	GLuint texture;

	GLboolean checkCollision(const GameObject& other);

	int getCollisionSide(const GameObject& other);
};