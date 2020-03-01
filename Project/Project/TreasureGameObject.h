#pragma once

#include "GravityGameObject.h"

class TreasureGameObject : public GravityGameObject {
private:
	int value;

public:
	TreasureGameObject(int entityValue, glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements);

	inline int getValue() { return value; }

	// Variables that give the texture IDs for the texture of the entity.
	static GLuint treasureTextureID;

	// Static method to load the treasure textures.
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};