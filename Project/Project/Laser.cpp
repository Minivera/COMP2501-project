#include "Laser.h"

#include "LaserGameObject.h"

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

void Laser::attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>>& gameObjects) {
	glm::vec3 laserTip = glm::vec3(
		cos(glm::radians(angle)) * 0.2,
		sin(glm::radians(angle)) * 0.2,
		0
	);

	gameObjects.push_back(make_shared<LaserGameObject>(
		damage,
		0.25,
		size,
		position + laserTip,
		angle
		));
	Weapon::attack(position, angle, gameObjects);
}