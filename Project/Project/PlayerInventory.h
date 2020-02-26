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
	unique_ptr<Harpoon> harpoon = make_unique<Harpoon>();
	unique_ptr<Pistol> pistol = make_unique<Pistol>();
	unique_ptr<Laser> laser = make_unique<Laser>();

	double baseAir = 2.0f * 60; // 2 minutes of air by default

	Weapon* equipedWeapon = harpoon.get();
public:
	// Method that unlocks a specific weapon.
	void unlock(WeaponType type);

	// Method that upgrades a specific weapon.
	void upgrade(WeaponType type);

	// Method that makes the inventory equip the specific weapon type.
	void equip(WeaponType type);

	vector<reference_wrapper<Weapon>> getWeapons();

	// Getters
	inline Weapon& getEquipedWeapon() { return *equipedWeapon; }
	WeaponType getEquipedWeaponType();
};

