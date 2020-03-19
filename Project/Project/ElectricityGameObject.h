#pragma once

#include "GameObject.h"

class ElectricityGameObject : public GameObject
{
private:
	double lifespan;
	glm::vec3 speed;
public:
	ElectricityGameObject(double entityLifespan, glm::vec3& entitySpeed, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements);

	// Updates the projectile to make it disapear once it reaches the end of its lifespan and have it collide with the player
	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	static GLuint projectileTextureID;

	// Static method to load the weapon textures from the file system
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};

