#include "Laser.h"

#include "LaserGameObject.h"

Laser::Laser(GLuint laserTexture, GLint entityNumElements): Weapon(false, baseDamage, baseSpeed, baseRechargeRate) {
	texture = laserTexture;
	numElements = entityNumElements;

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
	gameObjects.push_back(make_shared<LaserGameObject>(
		damage,
		0.25,
		size,
		position,
		angle,
		texture,
		numElements
		));
	Weapon::attack(position, angle, gameObjects);
}