#pragma once

#include "GameObject.h"

class BulletGameObject: public GameObject
{
private:
	double lifespan;
	glm::vec3 speed;

public:
	BulletGameObject(double entityLifespan, glm::vec3& entitySpeed, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements);

	// Updates the bullet to make it disapear once it reaches the end of its lifespan and have it collide with ennemies
	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);
};

