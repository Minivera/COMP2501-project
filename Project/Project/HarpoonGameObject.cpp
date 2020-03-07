#include "HarpoonGameObject.h"

HarpoonGameObject::HarpoonGameObject(int weaponDamage, double entityLifespan, glm::vec3& entityPosition) :
	WeaponGameObject(weaponDamage, entityPosition, 0, 0) {
	lifespan = entityLifespan;
}

void HarpoonGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	lifespan -= deltaTime;
	if (lifespan <= 0) {
		dirty = true;
	}

	GameObject::update(entities, deltaTime);
}

void HarpoonGameObject::render(Shader& spriteShader) {
	// Do not render anything, this entity exists only for collision detection
	return;
}
