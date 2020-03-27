#include "UpgradeTableGameObject.h"

#include "BoxGameObject.h"
#include "TextGameObject.h"
#include "IconGameObject.h"
#include "TreasureCounterGameObject.h"
#include "AirCounterGameObject.h"

#include "WeaponGameObject.h"
#include "TreasureGameObject.h"

#include "Harpoon.h"
#include "Pistol.h"
#include "Laser.h"

UpgradeTableGameObject::UpgradeTableGameObject(shared_ptr<PlayerGameObject> player, glm::vec3 entityPos, glm::vec3& entityScale, GLint entityNumElements):
	GameObject(entityPos, entityScale, 0, 0, entityNumElements) {
	this->player = player;

	loadInterface();
}

void UpgradeTableGameObject::loadInterface() {
	float xIncrement = 0.9f;
	float yIncrement = 0.65f;

	glm::vec3 boxScale = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 textScale = glm::vec3(0.3f, 0.2f, 1.0f);
	glm::vec3 markerScale = glm::vec3(0.2f, 0.2f, 1.0f);
	glm::vec3 counterScale = glm::vec3(0.6f, 0.6f, 1.0f);
	glm::vec3 boxPos = position + glm::vec3(-1.3f, 2.5f, 0.0f);
	glm::vec3 textPos = boxPos + glm::vec3(0.0f, 0.1f, -0.1f);
	glm::vec3 markerPos = boxPos + glm::vec3(0.0f, -0.1f, -0.1f);

	// Create and bind all the entities needed to render the interface.
	auto unlockHarpoon = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto unlockHarpoonText = make_shared<TextGameObject>(
		"unlock",
		textPos,
		textScale,
		6
	);
	auto unlockHarpoonMarker = make_shared<IconGameObject>(
		markerPos,
		markerScale,
		90.f,
		WeaponGameObject::harpoonTextureID,
		6
	);
	unlockHarpoon->complete();
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeHarpoon1 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradeHarpoon1Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradeHarpoon1Marker = make_shared<TextGameObject>(
		"+1",
		markerPos,
		markerScale,
		6
	);
	upgradeHarpoon1->onClick(std::bind(&UpgradeTableGameObject::upgradeHarpoon1, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeHarpoon2 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradeHarpoon2Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradeHarpoon2Marker = make_shared<TextGameObject>(
		"+2",
		markerPos,
		markerScale,
		6
	);
	upgradeHarpoon2->onClick(std::bind(&UpgradeTableGameObject::upgradeHarpoon2, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeHarpoon3 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradeHarpoon3Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradeHarpoon3Marker = make_shared<TextGameObject>(
		"+3",
		markerPos,
		markerScale,
		6
	);
	upgradeHarpoon3->onClick(std::bind(&UpgradeTableGameObject::upgradeHarpoon3, this, std::placeholders::_1));
	boxPos.x -= xIncrement * 3;
	textPos.x -= xIncrement * 3;
	markerPos.x -= xIncrement * 3;
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	// Skip the harpoon unlock gold cost
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeHarpoon1Cost = make_shared<TreasureCounterGameObject>(
		Harpoon::costByLevel,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeHarpoon2Cost = make_shared<TreasureCounterGameObject>(
		Harpoon::costByLevel * 2,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeHarpoon3Cost = make_shared<TreasureCounterGameObject>(
		Harpoon::costByLevel * 3,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x -= xIncrement * 3;
	textPos.x -= xIncrement * 3;
	markerPos.x -= xIncrement * 3;
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	auto unlockPistol = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto unlockPistolText = make_shared<TextGameObject>(
		"unlock",
		textPos,
		textScale,
		6
	);
	auto unlockPistolMarker = make_shared<IconGameObject>(
		markerPos,
		markerScale,
		-90.0f,
		WeaponGameObject::pistolTextureID,
		6
	);
	unlockPistol->onClick(std::bind(&UpgradeTableGameObject::unlockPistol, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradePistol1 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradePistol1Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradePistol1Marker = make_shared<TextGameObject>(
		"+1",
		markerPos,
		markerScale,
		6
	);
	upgradePistol1->onClick(std::bind(&UpgradeTableGameObject::upgradePistol1, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradePistol2 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradePistol2Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradePistol2Marker = make_shared<TextGameObject>(
		"+2",
		markerPos,
		markerScale,
		6
	);
	upgradePistol2->onClick(std::bind(&UpgradeTableGameObject::upgradePistol2, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradePistol3 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradePistol3Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradePistol3Marker = make_shared<TextGameObject>(
		"+3",
		markerPos,
		markerScale,
		6
	);
	upgradePistol3->onClick(std::bind(&UpgradeTableGameObject::upgradePistol3, this, std::placeholders::_1));
	boxPos.x -= xIncrement * 3;
	textPos.x -= xIncrement * 3;
	markerPos.x -= xIncrement * 3;
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	auto unlockPistolCost = make_shared<TreasureCounterGameObject>(
		Pistol::costByLevel,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradePistol1Cost = make_shared<TreasureCounterGameObject>(
		Pistol::costByLevel,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradePistol2Cost = make_shared<TreasureCounterGameObject>(
		Pistol::costByLevel * 2,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradePistol3Cost = make_shared<TreasureCounterGameObject>(
		Pistol::costByLevel * 3,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x -= xIncrement * 3;
	textPos.x -= xIncrement * 3;
	markerPos.x -= xIncrement * 3;
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	auto unlockLaser = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto unlockLaserText = make_shared<TextGameObject>(
		"unlock",
		textPos,
		textScale,
		6
	);
	auto unlockLaserMarker = make_shared<IconGameObject>(
		markerPos,
		markerScale,
		-90.0f,
		WeaponGameObject::laserTextureID,
		6
	);
	unlockLaser->onClick(std::bind(&UpgradeTableGameObject::unlockLaser, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeLaser1 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradeLaser1Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradeLaser1Marker = make_shared<TextGameObject>(
		"+1",
		markerPos,
		markerScale,
		6
	);
	upgradeLaser1->onClick(std::bind(&UpgradeTableGameObject::upgradeLaser1, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeLaser2 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradeLaser2Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradeLaser2Marker = make_shared<TextGameObject>(
		"+2",
		markerPos,
		markerScale,
		6
	);
	upgradeLaser2->onClick(std::bind(&UpgradeTableGameObject::upgradeLaser2, this, std::placeholders::_1));
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeLaser3 = make_shared<BoxGameObject>(
		boxPos,
		boxScale,
		6
	);
	auto upgradeLaser3Text = make_shared<TextGameObject>(
		"level",
		textPos,
		textScale,
		6
	);
	auto upgradeLaser3Marker = make_shared<TextGameObject>(
		"+3",
		markerPos,
		markerScale,
		6
	);
	upgradeLaser3->onClick(std::bind(&UpgradeTableGameObject::upgradeLaser3, this, std::placeholders::_1));
	boxPos.x -= xIncrement * 3;
	textPos.x -= xIncrement * 3;
	markerPos.x -= xIncrement * 3;
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	auto unlockLaserCost = make_shared<TreasureCounterGameObject>(
		Laser::costByLevel,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeLaser1Cost = make_shared<TreasureCounterGameObject>(
		Laser::costByLevel,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeLaser2Cost = make_shared<TreasureCounterGameObject>(
		Laser::costByLevel * 2,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x += xIncrement;
	textPos.x += xIncrement;
	markerPos.x += xIncrement;

	auto upgradeLaser3Cost = make_shared<TreasureCounterGameObject>(
		Laser::costByLevel * 3,
		boxPos,
		counterScale,
		TreasureGameObject::treasureTextureID,
		6
	);
	boxPos.x -= xIncrement * 3;
	textPos.x -= xIncrement * 3;
	markerPos.x -= xIncrement * 3;
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	subEntities.push_back(unlockHarpoon);
	subEntities.push_back(unlockHarpoonText);
	subEntities.push_back(unlockHarpoonMarker);

	subEntities.push_back(upgradeHarpoon1);
	subEntities.push_back(upgradeHarpoon1Text);
	subEntities.push_back(upgradeHarpoon1Marker);

	subEntities.push_back(upgradeHarpoon2);
	subEntities.push_back(upgradeHarpoon2Text);
	subEntities.push_back(upgradeHarpoon2Marker);

	subEntities.push_back(upgradeHarpoon3);
	subEntities.push_back(upgradeHarpoon3Text);
	subEntities.push_back(upgradeHarpoon3Marker);

	subEntities.push_back(upgradeHarpoon1Cost);
	subEntities.push_back(upgradeHarpoon2Cost);
	subEntities.push_back(upgradeHarpoon3Cost);

	subEntities.push_back(unlockPistol);
	subEntities.push_back(unlockPistolText);
	subEntities.push_back(unlockPistolMarker);

	subEntities.push_back(upgradePistol1);
	subEntities.push_back(upgradePistol1Text);
	subEntities.push_back(upgradePistol1Marker);

	subEntities.push_back(upgradePistol2);
	subEntities.push_back(upgradePistol2Text);
	subEntities.push_back(upgradePistol2Marker);

	subEntities.push_back(upgradePistol3);
	subEntities.push_back(upgradePistol3Text);
	subEntities.push_back(upgradePistol3Marker);

	subEntities.push_back(unlockPistolCost);
	subEntities.push_back(upgradePistol1Cost);
	subEntities.push_back(upgradePistol2Cost);
	subEntities.push_back(upgradePistol3Cost);

	subEntities.push_back(unlockLaser);
	subEntities.push_back(unlockLaserText);
	subEntities.push_back(unlockLaserMarker);

	subEntities.push_back(upgradeLaser1);
	subEntities.push_back(upgradeLaser1Text);
	subEntities.push_back(upgradeLaser1Marker);

	subEntities.push_back(upgradeLaser2);
	subEntities.push_back(upgradeLaser2Text);
	subEntities.push_back(upgradeLaser2Marker);

	subEntities.push_back(upgradeLaser3);
	subEntities.push_back(upgradeLaser3Text);
	subEntities.push_back(upgradeLaser3Marker);

	subEntities.push_back(unlockLaserCost);
	subEntities.push_back(upgradeLaser1Cost);
	subEntities.push_back(upgradeLaser2Cost);
	subEntities.push_back(upgradeLaser3Cost);
	
	// Load the player information
	treasureCounter = make_shared<TreasureCounterGameObject>(
		player->getCurrentTreasure(),
		glm::vec3(boxPos.x + (xIncrement * 3) / 2, boxPos.y, boxPos.z),
		glm::vec3(1.0f, 1.0f, 1.0f),
		TreasureGameObject::treasureTextureID,
		6
	);
	subEntities.push_back(treasureCounter);
	boxPos.y -= yIncrement;
	textPos.y -= yIncrement;
	markerPos.y -= yIncrement;

	airCounter = make_shared<AirCounterGameObject>(
		player->getBaseAir(),
		glm::vec3(boxPos.x + (xIncrement * 3) / 2, boxPos.y, boxPos.z),
		glm::vec3(1.0f, 1.0f, 1.0f),
		TextGameObject::textureFor("air"),
		6
	);
	subEntities.push_back(airCounter);
}

void UpgradeTableGameObject::upgradeHarpoon1(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Harpoon)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradeHarpoon2(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Harpoon)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradeHarpoon3(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Harpoon)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::unlockPistol(GuiGameObject& caller) {
	if (player->unlock(WeaponType::Pistol)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradePistol1(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Pistol)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradePistol2(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Pistol)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradePistol3(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Pistol)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::unlockLaser(GuiGameObject& caller) {
	if (player->unlock(WeaponType::Laser)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradeLaser1(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Laser)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradeLaser2(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Laser)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::upgradeLaser3(GuiGameObject& caller) {
	if (player->upgrade(WeaponType::Laser)) {
		dynamic_cast<BoxGameObject&>(caller).complete();
	}
}

void UpgradeTableGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// Update things based on the player object
	treasureCounter->setAmount(player->getCurrentTreasure());

	airCounter->setAmount(player->getBaseAir());

	GameObject::update(entities, deltaTime);
}

void UpgradeTableGameObject::render(Shader& spriteShader) {
	for (auto it = subEntities.begin(); it != subEntities.end(); it++) {
		(*it)->render(spriteShader);
	}
}

void UpgradeTableGameObject::clean() {
	for (auto it = subEntities.begin(); it != subEntities.end(); it++) {
		(*it)->clean();
	}

	GameObject::clean();
}
