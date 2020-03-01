#pragma once

#include "EnemyGameObject.h"

class JellyfishGameObject: public EnemyGameObject
{
public:
	JellyfishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements);
};


