#include "Pistol.h"

#include "BulletGameObject.h"

Pistol::Pistol(): Weapon(false, baseDamage, baseSpeed, baseRechargeRate) {
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
	gameObjects.push_back(make_shared<BulletGameObject>(
		range,
		glm::vec3(speed, 0, 0),
		position,
		angle,
		8,
		6
	));
	Weapon::attack(position, angle, gameObjects);
}