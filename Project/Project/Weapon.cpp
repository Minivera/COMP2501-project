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

void Weapon::attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>> gameObjects) {
	return;
}

void Weapon::recharge(double deltaTime) {
	// Stop reloading 
	if (attackReady()) {
		return;
	}
	timeSinceAttack = +deltaTime;
}

int Weapon::setWeaponTexture(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\objects\\trident-single.png");
	setFuncPtr(textures[offset + 1], "Assets\\objects\\pistol.png");
	setFuncPtr(textures[offset + 2], "Assets\\objects\\laser.png");
	return offset + 3;
}