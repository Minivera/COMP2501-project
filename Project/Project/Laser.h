#pragma once

#include "Weapon.h"

class Laser: public Weapon
{
private:
	// How large the laser is meant to be
	double size = 0.15;

	const int baseDamage = 5;
	const double baseSpeed = 150;
	const double baseRechargeRate = 1.75;

	// Multipliers to apply when leveling up the harpoon
	const double rechargeRateByLevel = 0.85;
	const double sizeByLevel = 1.5;
	const double damageByLevel = 1.5;
public:
	Laser();

	// Method to upgrade the weapon's level
	virtual void upgrade();

	// Method to trigger the weapon's attack. Does not handle the weapon's fring rate (Speed).
	virtual void attack(glm::vec3 position, double angle, double boost, std::vector<shared_ptr<GameObject>>& gameObjects);

	// The cost to unlock and upgrade the weapon.
	static const int costByLevel = 500;
};

