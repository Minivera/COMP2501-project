#include "BulletGameObject.h"

BulletGameObject::BulletGameObject(int weaponDamage, double entityLifespan, glm::vec3& entitySpeed, glm::vec3& entityPosition, GLfloat entityAngle, GLuint entityTexture, GLint entityNumElements):
	WeaponGameObject(weaponDamage, entityPosition, entitySpeed, glm::vec3(0.05, 0.05, 1), entityAngle, entityTexture, entityNumElements) {
	lifespan = entityLifespan;
	angle = entityAngle;
	speed = entitySpeed;
}

void BulletGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	lifespan -= speed.x * deltaTime;
	if (lifespan <= 0) {
		dirty = true;
	}

	velocity.x = speed.x * deltaTime;

	GameObject::update(entities, deltaTime);
}
