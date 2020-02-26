#include "Pistol.h"

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

void Pistol::attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>> gameObjects) {
	// TODO: Render a game object
	return;
}