#pragma once

#include <vector>
#include <memory>

#include "Harpoon.h"
#include "Pistol.h"
#include "Laser.h"
#include "TreasureGameObject.h"
#include "PowerupGameObject.h"

constexpr float AIR_INCREASE = 1.5f;

enum class WeaponType { Harpoon, Pistol, Laser };

class PlayerInventory
{
private:
	unique_ptr<Harpoon> harpoon;
	unique_ptr<Pistol> pistol;
	unique_ptr<Laser> laser;

	double baseAir = 2.0 * 60.0; // 2 minutes of air by default

	Weapon* equipedWeapon;

	// Attribute that counts how much treasure the player has available.
	int currentTreasure = 1500; // Start the player with some money

	// Attribute that counts the current air from the player in seconds.
	double currentAir = baseAir;

	// How much treasure is lost when getting hurt.
	int treasureLossFactor = 30;

	// How much air is lost when getting hurt.
	double airLossFactor = 0.10; // 10 second by default

	// The current powerup in inventory. Will always be set, but might not be active.
	Powerup& currentPowerup = Powerup{};

	// How long the current powerup will be active. -1 if inactive.
	double powerupTimer = -1;
public:
	PlayerInventory(GLuint bulletTexture, GLint entityNumElements);

	// Method that unlocks a specific weapon.
	bool unlock(WeaponType type);

	// Method that upgrades a specific weapon.
	bool upgrade(WeaponType type);

	// Method that makes the inventory equip the specific weapon type.
	void equip(WeaponType type);

	// Runs an update mechanism on all elements of the inventory that runs on a timer, like air.
	void update(double deltaTime);

	// Methods that can add objects to the inventory. Overload to add more behavior.
	void addToInventory(shared_ptr<TreasureGameObject> treasure);
	void addToInventory(shared_ptr<PowerupGameObject> powerup);

	// Method that makes the inventory lose a set amount of treasure and air
	int loseTreasure();
	void loseAir();

	// Returns all the weapons currently available to the player
	vector<reference_wrapper<Weapon>> getWeapons();

	// Getters
	inline Weapon& getEquipedWeapon() { return *equipedWeapon; }
	inline int getTreasure() { return currentTreasure; }
	inline double getBaseAir() { return baseAir; }
	inline double getAir() { return currentAir; }
	inline Powerup& getPowerup() { return currentPowerup; }
	inline double getPowerupTimer() { return powerupTimer; }
	WeaponType getEquipedWeaponType();

	// Powerup related getters
	inline double damageBoost() { return currentPowerup.type == PowerupType::Pepper && powerupTimer > 0 ? currentPowerup.effect : 0; }
	inline bool isInvulnerable() { return currentPowerup.type == PowerupType::Armor && powerupTimer > 0; }
};

