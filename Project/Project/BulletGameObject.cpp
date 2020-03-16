#include "BulletGameObject.h"

#include "TerrainGameObject.h"

BulletGameObject::BulletGameObject(int weaponDamage, double entityLifespan, glm::vec3& entitySpeed, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements):
	WeaponGameObject(weaponDamage, entityPosition, entitySpeed, glm::vec3(0.05, 0.05, 1), entityAngle, entityTexture, entityNumElements) {
	lifespan = entityLifespan;
	angle = entityAngle;
	speed = entitySpeed;
}

void BulletGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a wall
		auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (terrain && checkCollision(*(*it))) {
			dirty = true;
			return;
		}
	}

	lifespan -= speed.x * deltaTime;
	if (lifespan <= 0) {
		dirty = true;
	}

	velocity.x = speed.x * deltaTime;

	GameObject::update(entities, deltaTime);
}
