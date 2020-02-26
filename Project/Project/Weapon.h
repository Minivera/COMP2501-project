#pragma once

#include <vector>
#include <memory>

#include "GameObject.h"

// This class includes the state and behavior for a player's weapon, but not the entity tied to that weapon.
class Weapon
{
protected:
	// Base attributes common to all weapons
	bool enabled;
	int damage;
	double speed;
	double rechargeRate;
	int level = 1;

	// Max time constant
	const int maxLevel = 3;

	// Counter that counts since the last shot
	double timeSinceAttack = 0;
public:
	Weapon(bool initEnabled, int initDamage, double initSpeed, double initRechargeRate);

	// Method to upgrade the weapon's level
	virtual void upgrade();

	// Method to trigger the weapon's attack. Does not handle the weapon's fring rate (Speed).
	virtual void attack(glm::vec3 position, double angle, std::vector<shared_ptr<GameObject>> gameObjects);

	// Method that prepares the reload for the weapon by adding the delta time to a counter
	void recharge(double deltaTime);

	// Checks if the weapon is ready to be used.
	inline bool attackReady() { return timeSinceAttack >= rechargeRate; }

	// Fully reloads the weapon, ignoring any counter.
	inline void reload() { timeSinceAttack = rechargeRate; }

	// Getters
	inline bool isUpgradable() { return level < maxLevel; }
	inline bool isEnabled() { return enabled; }
	inline int getDamage() { return damage; }
	inline float getSpeed() { return speed; }
	inline float getRechargeRate() { return rechargeRate; }
	inline int getLevel() { return level; }

	// Setters
	inline void unlock() { enabled = true; }

	// Static method to load the weapon textures from the file system
	static int setWeaponTexture(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};

