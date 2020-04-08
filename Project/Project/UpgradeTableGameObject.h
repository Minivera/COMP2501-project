#pragma once

#include "GameObject.h"

#include "PlayerGameObject.h"
#include "GuiGameObject.h"
#include "TreasureCounterGameObject.h"
#include "AirCounterGameObject.h"

class UpgradeTableGameObject : public GameObject
{
private:
	// Reference to the player to update the inventory when necessary.
	shared_ptr<PlayerGameObject> player;

	// Reference to the treasure counter in the interface that should update when the players gains more treasure.
	shared_ptr<TreasureCounterGameObject> treasureCounter;

	// Reference to the air counter in the interface that should update when the players gains more base air.
	shared_ptr<AirCounterGameObject> airCounter;

	// References to the cost counters for the infinite suit upgrades
	shared_ptr<TreasureCounterGameObject> upgradeAirCost;
	shared_ptr<TreasureCounterGameObject> upgradeSuitCost;
	shared_ptr<TreasureCounterGameObject> upgradeFlippersCost;

	// Vector with all the entities to render within this entity.
	vector<shared_ptr<GameObject>> subEntities;

	// Method that loads the interface for the upgrade table;
	void loadInterface();

	// Upgrade methods to bind to the event listeners of the various boxes.
	void upgradeHarpoon1(GuiGameObject& caller);
	void upgradeHarpoon2(GuiGameObject& caller);
	void upgradeHarpoon3(GuiGameObject& caller);
	void unlockPistol(GuiGameObject& caller);
	void upgradePistol1(GuiGameObject& caller);
	void upgradePistol2(GuiGameObject& caller);
	void upgradePistol3(GuiGameObject& caller);
	void unlockLaser(GuiGameObject& caller);
	void upgradeLaser1(GuiGameObject& caller);
	void upgradeLaser2(GuiGameObject& caller);
	void upgradeLaser3(GuiGameObject& caller);
	void upgradeAir(GuiGameObject& caller);
	void upgradeSuit(GuiGameObject& caller);
	void upgradeFlippers(GuiGameObject& caller);
public:
	UpgradeTableGameObject(shared_ptr<PlayerGameObject> player, glm::vec3 entityPos, glm::vec3& entityScale, GLint entityNumElements);

	void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	void render(Shader& spriteShader);
	
	void clean();

	// Getters
	inline vector<shared_ptr<GameObject>>& getSubEntities() { return subEntities; }
};

