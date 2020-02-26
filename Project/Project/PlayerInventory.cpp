#include "PlayerInventory.h"

void PlayerInventory::unlock(WeaponType type) {
	switch (type)
	{
	case WeaponType::Harpoon:
		harpoon->unlock();
		break;
	case WeaponType::Pistol:
		pistol->unlock();
		break;
	case WeaponType::Laser:
		laser->unlock();
		break;
	}
}

void PlayerInventory::upgrade(WeaponType type) {
	switch (type)
	{
	case WeaponType::Harpoon:
		if (harpoon->isEnabled() && harpoon->isUpgradable()) {
			harpoon->upgrade();
		}
		break;
	case WeaponType::Pistol:
		if (pistol->isEnabled() && pistol->isUpgradable()) {
			pistol->upgrade();
		}
		break;
	case WeaponType::Laser:
		if (laser->isEnabled() && laser->isUpgradable()) {
			laser->upgrade();
		}
		break;
	}
}

void PlayerInventory::equip(WeaponType type) {
	switch (type)
	{
	case WeaponType::Harpoon:
		if (harpoon->isEnabled()) {
			equipedWeapon = harpoon.get();
			equipedWeapon->reload();
		}
		break;
	case WeaponType::Pistol:
		if (pistol->isEnabled()) {
			equipedWeapon = pistol.get();
			equipedWeapon->reload();
		}
		break;
	case WeaponType::Laser:
		if (laser->isEnabled()) {
			equipedWeapon = laser.get();
			equipedWeapon->reload();
		}
		break;
	}
}

vector<reference_wrapper<Weapon>> PlayerInventory::getWeapons() {
	vector<reference_wrapper<Weapon>> weapons = vector<reference_wrapper<Weapon>>();

	if (harpoon->isEnabled()) {
		weapons.push_back(ref<Weapon>(*harpoon.get()));
	}
	if (pistol->isEnabled()) {
		weapons.push_back(ref<Weapon>(*pistol.get()));
	}
	if (laser->isEnabled()) {
		weapons.push_back(ref<Weapon>(*laser.get()));
	}

	return weapons;
}

WeaponType PlayerInventory::getEquipedWeaponType() {
	if (dynamic_cast<Harpoon*>(equipedWeapon)) {
		return WeaponType::Harpoon;
	} else if (dynamic_cast<Pistol*>(equipedWeapon)) {
		return WeaponType::Pistol;
	} else if (dynamic_cast<Laser*>(equipedWeapon)) {
		return WeaponType::Laser;
	}
	return WeaponType::Harpoon;
}