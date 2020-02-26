#pragma once

#include "Weapon.h"

class Harpoon: public Weapon
{
private:
	const int baseDamage = 5;
	const double baseSpeed = 100;
	const double baseRechargeRate = 0.30;

	// Multipliers to apply when leveling up the harpoon
	const double rechargeRateByLevel = 0.85f;
	const double damageByLevel = 1.5f;
public:
	Harpoon();

	// Method to upgrade the weapon's level
	virtual void upgrade();

	// Method to trigger the weapon's attack. Does not handle the weapon's fring rate (Speed).
	virtual void attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>>& gameObjects);
};

