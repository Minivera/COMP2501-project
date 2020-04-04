#pragma once

#include "GameObject.h"

class GravityGameObject: public GameObject {
protected:
	// Defines the global acceleration of gravity for all gravity based entities.
	const GLfloat gravityAccel = -9.8f;

	// Defines if the gravity is activate on this entity
	bool gravityActivated = true;

	GravityGameObject(glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);
	GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
	GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
public:
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);
};