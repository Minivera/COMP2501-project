#pragma once

#include <vector>
#include <memory>

#include "Harpoon.h"
#include "Pistol.h"
#include "Laser.h"
#include "TreasureGameObject.h"
#include "PowerupGameObject.h"

enum class WeaponType { Harpoon, Pistol, Laser };
enum class UpgradeType { Harpoon, Pistol, Laser, Suit, Flippers, Air };

class PlayerInventory
{
private:
	// Constants that define the various amount to add for each update
	const double airIncrease = 30.0f; // Adds 30 second of air per upgrade
	const double treasureLossDecrease = 0.5f; // lose 5 less treasure on hit

	// Constants that define the current level for the player
	int airLevel = 1;
	int suitLevel = 1;
	int flippersLevel = 1;

	unique_ptr<Harpoon> harpoon;
	unique_ptr<Pistol> pistol;
	unique_ptr<Laser> laser;

	// The base level of air the player gets when starting a new dive
	double baseAir = 2.0 * 60.0; // 2 minutes of air by default

	// The currently equiped weapon for the player
	Weapon* equipedWeapon;

	// Attribute that counts how much treasure the player has available.
	int currentTreasure = 150; // Start the player with some money

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
	bool upgrade(UpgradeType type);

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
	inline Weapon& getEquipedWeapon() const { return *equipedWeapon; }
	inline int getTreasure() const { return currentTreasure; }
	inline double getBaseAir() const { return baseAir; }
	inline double getAir() const { return currentAir; }
	inline Powerup& getPowerup() const { return currentPowerup; }
	inline double getPowerupTimer() const { return powerupTimer; }
	WeaponType getEquipedWeaponType();

	// Upgrade related getters
	inline int getAirLevel() const{ return airLevel; }
	inline int getSuitLevel() const{ return suitLevel; }
	inline int getFlippersLevel() const{ return flippersLevel; }

	// Powerup related getters
	inline double damageBoost() const { return currentPowerup.type == PowerupType::Pepper && powerupTimer > 0 ? currentPowerup.effect : 0; }
	inline bool isInvulnerable() const { return currentPowerup.type == PowerupType::Armor && powerupTimer > 0; }

	// Constants that define the cost of each upgrades
	static const int airCost = 300;
	static const int treasureLossCost = 250;
	static const int flipperCost = 500;
};

