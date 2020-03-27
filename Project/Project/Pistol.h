#pragma once

#include "Weapon.h"

class Pistol : public Weapon
{
private:
	GLuint texture;
	GLint numElements;

	// How far the bullet will go.
	double range = 200;

	const int baseDamage = 5;
	const double baseSpeed = 150;
	const double baseRechargeRate = 0.15;

	// Multipliers to apply when leveling up the harpoon
	const double rangeByLevel = 1.25f;
	const double damageByLevel = 1.5f;
public:
	Pistol(GLuint bulletTexture, GLint entityNumElements);

	// Method to upgrade the weapon's level
	virtual void upgrade();

	// Method to trigger the weapon's attack. Does not handle the weapon's fring rate (Speed).
	virtual void attack(glm::vec3 position, double angle, double boost, std::vector<shared_ptr<GameObject>>& gameObjects);

	// The cost to unlock and upgrade the weapon.
	static const int costByLevel = 250;
};

