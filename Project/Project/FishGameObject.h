#pragma once

#include "EnemyGameObject.h"

class FishGameObject: public EnemyGameObject
{
private:
	// Range at which this entity will start chasing the player.
	double viewRange = 6;
public:
	FishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements);

	// Overriden update method to have the fish chae the player when required
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);
};

