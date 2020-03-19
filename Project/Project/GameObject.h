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

enum class CollisionSides { Top, Bottom, Left, Right };

class GameObject {
public:
	// Updates the GameObject's state. Can be overriden for children
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Renders the GameObject using a shader
	virtual void render(Shader& spriteShader);

	// Renders the GameObject's particle system using a shader
	virtual void renderParticles(Shader& particlesShader);

	// Debug method that render an object's bounding box.
	void renderBoundingBox(Shader& spriteShader);

	// Cleans the object after an update has occured
	virtual void clean();

	// Check if the game object collides with another game object
	GLboolean checkCollision(const GameObject& other);

	// Getters
	inline const glm::vec3& getPosition() const { return position; };
	inline const glm::vec3& getVelocity() const { return velocity; }
	inline GLfloat getAngle() const { return angle; }
	inline const glm::vec2& getBoundingBox() const { return boundingBox; };
	inline bool isDirty() const { return dirty; }

	// Setters
	inline void setPosition(glm::vec3& newPosition) { position = newPosition; }
	inline void setVelocity(glm::vec3& newVelocity) { velocity = newVelocity; }
	inline void setAngle(GLfloat newAngle) { angle = newAngle; }
	inline void setRotation(GLfloat newRotation) { rotation = newRotation; }
	inline void setDirty(bool newDirty) { dirty = newDirty; }

	// Variables that give the texture IDs for the texture of the entity.
	static GLuint boundingBoxTextureID;

	// Static method to load the treasure textures.
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
protected:
	GameObject(glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);
	GameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
	GameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);

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

	// Defines the bounding box of the entity for the purposes of collision detection
	glm::vec2 boundingBox;

	CollisionSides getCollisionSide(const GameObject& other);
};