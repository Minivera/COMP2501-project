#include "Harpoon.h"

#include "HarpoonGameObject.h"

const int Harpoon::costByLevel;

// Harpoon is enabled by default
Harpoon::Harpoon(): Weapon(true, baseDamage, baseSpeed, baseRechargeRate) {
	// Constants seem to not work when called in super
	damage = baseDamage;
	speed = baseSpeed;
	rechargeRate = baseRechargeRate;
}

void Harpoon::upgrade() {
	Weapon::upgrade();
	if (!isUpgradable()) {
		return;
	}
	damage += damage * damageByLevel;
	rechargeRate -= rechargeRate * rechargeRateByLevel;
}

void Harpoon::attack(glm::vec3 position, double angle, double boost, std::vector<shared_ptr<GameObject>>& gameObjects) {
	glm::vec3 harpoonTip = glm::vec3(
		cos(glm::radians(angle)) * 0.5,
		sin(glm::radians(angle)) * 0.5,
		0
	);

	gameObjects.push_back(make_shared<HarpoonGameObject>(
		damage + damage * boost,
		0.15,
		position + harpoonTip
	));
	Weapon::attack(position, angle, boost, gameObjects);
}