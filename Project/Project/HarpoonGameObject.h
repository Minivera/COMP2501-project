#pragma once

#include "WeaponGameObject.h"

class HarpoonGameObject: public WeaponGameObject
{
private:
	double lifespan;

public:
	HarpoonGameObject(int weaponDamage, double entityLifespan, glm::vec3& entityPosition);

	// Updates the harpoon to make it disapear once it reaches the end of its lifespan and have it collide with ennemies
	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// We use the skyrim trick of rendering an invisible object to hurt enemies, so we override render to render nothing.
	void render(Shader& spriteShader);
};


