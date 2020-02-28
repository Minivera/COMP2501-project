#pragma once

#include "GravityGameObject.h"

class TreasureGameObject : public GravityGameObject {
private:
	int value;

public:
	TreasureGameObject(int entityValue, glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);

	inline int getValue() { return value; }
};