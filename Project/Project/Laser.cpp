#include "Laser.h"

Laser::Laser(): Weapon(false, baseDamage, baseSpeed, baseRechargeRate) {
	// Constants seem to not work when called in super
	damage = baseDamage;
	speed = baseSpeed;
	rechargeRate = baseRechargeRate;
}

void Laser::upgrade() {
	Weapon::upgrade();
	if (!isUpgradable()) {
		return;
	}
	damage += damage * damageByLevel;
	rechargeRate -= rechargeRate * rechargeRateByLevel;
	size += size * sizeByLevel;
}

void Laser::attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>> gameObjects) {
	// TODO: Render a game object
	return;
}