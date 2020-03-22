#include "Weapon.h"

Weapon::Weapon(bool initEnabled, int initDamage, double initSpeed, double initRechargeRate) {
	enabled = initEnabled;
	damage = initDamage;
	speed = initSpeed;
	rechargeRate = initRechargeRate;
}

void Weapon::upgrade() {
	if (!isUpgradable()) {
		return;
	}
	level += 1;
}

void Weapon::attack(glm::vec3 position, double angle, double boost, std::vector<shared_ptr<GameObject>>& gameObjects) {
	return;
}

void Weapon::recharge(double deltaTime) {
	// Stop reloading 
	if (attackReady()) {
		return;
	}
	timeSinceAttack = +deltaTime;
}