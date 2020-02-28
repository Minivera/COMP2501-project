#pragma once

#include "EnemyGameObject.h"

class FishGameObject: public EnemyGameObject
{
private:
	double viewRange = 3;
public:
	FishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements);

	// Overriden update method to have the fish face the player at all times
	virtual void update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime);
};

