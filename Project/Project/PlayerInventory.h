#pragma once

#include <vector>
#include <memory>

#include "Harpoon.h"
#include "Pistol.h"
#include "Laser.h"

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
	int currentTreasure = 0;

	// Attribute that counts the current air from the player in seconds.
	double currentAir = baseAir;
public:
	PlayerInventory(GLuint bulletTexture, GLuint laserTexture, GLint entityNumElements);

	// Method that unlocks a specific weapon.
	void unlock(WeaponType type);

	// Method that upgrades a specific weapon.
	void upgrade(WeaponType type);

	// Method that makes the inventory equip the specific weapon type.
	void equip(WeaponType type);

	vector<reference_wrapper<Weapon>> getWeapons();

	// Getters
	inline Weapon& getEquipedWeapon() { return *equipedWeapon; }
	inline int getTreasure() { return currentTreasure; }
	inline double getAir() { return currentAir; }
	WeaponType getEquipedWeaponType();

	// Setters
	inline void addTreasure(int amount) { currentTreasure += amount; }
	inline void addAir(double amount) { currentAir += amount; }
	inline void removeTreasure(int amount) { currentTreasure -= amount; }
	inline void removeAir(double amount) { currentAir -= amount; }
};

