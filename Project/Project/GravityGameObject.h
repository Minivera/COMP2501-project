#pragma once

#include "GameObject.h"

class GravityGameObject: public GameObject {
protected:
	GLfloat gravityAccel = 1.6f;

	bool gravityActivated = true;

public:
	GravityGameObject(glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);
	GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);
	GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);

	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);
};