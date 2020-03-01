#include "Pistol.h"

#include "BulletGameObject.h"

Pistol::Pistol(GLuint bulletTexture, GLint entityNumElements): Weapon(false, baseDamage, baseSpeed, baseRechargeRate) {
	texture = bulletTexture;
	numElements = entityNumElements;

	// Constants seem to not work when called in super
	damage = baseDamage;
	speed = baseSpeed;
	rechargeRate = baseRechargeRate;
}

void Pistol::upgrade() {
	Weapon::upgrade();
	if (!isUpgradable()) {
		return;
	}
	damage += damage * damageByLevel;
	range += range * rangeByLevel;
}

void Pistol::attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>>& gameObjects) {
	glm::vec3 pistolTip = glm::vec3(
		cos(glm::radians(angle)) * 0.2,
		sin(glm::radians(angle)) * 0.2,
		0
	);

	gameObjects.push_back(make_shared<BulletGameObject>(
		damage,
		range,
		glm::vec3(speed, 0, 0),
		position + pistolTip,
		angle,
		texture,
		numElements
	));
	Weapon::attack(position, angle, gameObjects);
}