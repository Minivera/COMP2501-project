#pragma once

#include "WeaponGameObject.h"

class LaserGameObject: public WeaponGameObject
{
private:
	double baseLifespan;
	double lifespan;
	GLfloat size;

	// Method that checks if the laser hit an entity
	bool checkLaserHits(GameObject& entity);
public:
	LaserGameObject(int weaponDamage, double entityLifespan, GLfloat entitySize, glm::vec3& entityPosition, GLfloat entityAngle);

	// Updates the laser to make it disapear once it reaches the end of its lifespan and have it collide with ennemies
	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Render method that prevents the render of the laser since it's a particle
	void render(Shader& spriteShader);

	// Method that will render the laser as a particle so it is nicely animated
	void renderParticles(Shader& particleShader);
};

