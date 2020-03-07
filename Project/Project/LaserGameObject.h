#pragma once

#include "WeaponGameObject.h"

class LaserGameObject: public WeaponGameObject
{
private:
	double lifespan;
	GLfloat size;

public:
	LaserGameObject(int weaponDamage, double entityLifespan, GLfloat entitySize, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements);

	// Updates the laser to make it disapear once it reaches the end of its lifespan and have it collide with ennemies
	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Render method that displaces the laser so it appear next to its position
	void render(Shader& spriteShader);
};

