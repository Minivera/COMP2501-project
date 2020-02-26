#pragma once

#include "GravityGameObject.h"

class TreasureGameObject : public GravityGameObject {
private:
	int value;

public:
	TreasureGameObject(int entityValue, glm::vec3& entityPosition, glm::vec3& entityScale, GLuint entityTexture, GLint entityNumElements);

	inline int getValue() { return value; }
};