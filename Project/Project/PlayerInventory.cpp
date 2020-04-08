#include "PlayerInventory.h"

PlayerInventory::PlayerInventory(GLuint bulletTexture, GLint entityNumElements) {
	harpoon = make_unique<Harpoon>();
	pistol = make_unique<Pistol>(bulletTexture, entityNumElements);
	laser = make_unique<Laser>();

	equipedWeapon = harpoon.get();
}

bool PlayerInventory::unlock(WeaponType type) {
	switch (type)
	{
	case WeaponType::Harpoon:
		if (currentTreasure > Harpoon::costByLevel) {
			currentTreasure -= Harpoon::costByLevel;
			harpoon->unlock();
			return true;
		}
		break;
	case WeaponType::Pistol:
		if (currentTreasure > Pistol::costByLevel) {
			currentTreasure -= Pistol::costByLevel;
			pistol->unlock();
			return true;
		}
		break;
	case WeaponType::Laser:
		if (currentTreasure > Laser::costByLevel) {
			currentTreasure -= Laser::costByLevel;
			laser->unlock();
			return true;
		}
		break;
	}
	return false;
}

bool PlayerInventory::upgrade(UpgradeType type) {
	switch (type)
	{
	case UpgradeType::Harpoon:
		if (harpoon->isEnabled() && harpoon->isUpgradable() && currentTreasure > Harpoon::costByLevel * harpoon->getLevel()) {
			currentTreasure -= Harpoon::costByLevel * harpoon->getLevel();
			harpoon->upgrade();
			return true;
		}
		break;
	case UpgradeType::Pistol:
		if (pistol->isEnabled() && pistol->isUpgradable() && currentTreasure >= Pistol::costByLevel* pistol->getLevel()) {
			currentTreasure -= Pistol::costByLevel * pistol->getLevel();
			pistol->upgrade();
			return true;
		}
		break;
	case UpgradeType::Laser:
		if (laser->isEnabled() && laser->isUpgradable() && currentTreasure >= Laser::costByLevel* laser->getLevel()) {
			currentTreasure -= Laser::costByLevel * laser->getLevel();
			laser->upgrade();
			return true;
		}
		break;
	case UpgradeType::Air:
		if (currentTreasure >= airCost * airLevel) {
			currentTreasure -= airCost * airLevel;
			baseAir += airIncrease;
			airLevel++;
			return true;
		}
		break;
	case UpgradeType::Suit:
		if (currentTreasure >= treasureLossCost * suitLevel) {
			currentTreasure -= treasureLossCost * suitLevel;
			treasureLossFactor -= treasureLossDecrease;
			suitLevel++;
			return true;
		}
		break;
	case UpgradeType::Flippers:
		if (currentTreasure >= flipperCost * flippersLevel) {
			currentTreasure -= flipperCost * flippersLevel;
			flippersLevel++;
			return true;
		}
		break;
	}
	return false;
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

void PlayerInventory::update(double deltaTime) {
	currentAir -= deltaTime;
	powerupTimer -= deltaTime;

	// if the current powerup has expired, deactivate the powerup
	if (powerupTimer <= 0) {
		powerupTimer = -1;
	}
}

void PlayerInventory::addToInventory(shared_ptr<TreasureGameObject> treasureEntity) {
	currentTreasure += treasureEntity->getValue();
}

void PlayerInventory::addToInventory(shared_ptr<PowerupGameObject> powerupEntity) {
	currentPowerup = powerupEntity->getPowerup();

	if (currentPowerup.type == PowerupType::Air) {
		currentAir += currentPowerup.duration;
	}
	else {
		powerupTimer = currentPowerup.duration;
	}
}

int PlayerInventory::loseTreasure() {
	// If we got less than how much treasure we're set to lose, lose it all
	if (currentTreasure <= treasureLossFactor) {
		int treasureLost = currentTreasure;
		currentTreasure = 0;
		return treasureLost;
	}

	// Otherwise, lose our loss factor in treasure
	currentTreasure -= treasureLossFactor;
	return treasureLossFactor;
}

void PlayerInventory::loseAir() {
	currentAir -= airLossFactor;
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