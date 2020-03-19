#pragma once

#include "EnemyGameObject.h"

class JellyfishGameObject: public EnemyGameObject
{
private:
	// Range at which this entity will flee away from the player.
	double fleeRange = 2;

	// Range at which this entity will start attacking the player.
	double viewRange = 5;

	// The time to track the time between attacks.
	double attackTimer = 0;

	// How long the game should wait between attacks.
	double attackWaitTime = 1.5;

	// Speed at which the weapon moves.
	double attackSpeed = 100;

	// How many units the attack will live for.
	double attackRange = 350;

	// Method to make the jelly fish attack the player
	void attack(std::vector<shared_ptr<GameObject>>& entities);
public:
	JellyfishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements);

	// Overriden update method to have the jellyfish hurl electricity at the player
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);

	// Overriden clean method to clean anything the enemy needs cleaning.
	virtual void clean();
};


