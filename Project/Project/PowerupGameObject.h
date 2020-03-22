#pragma once

#include "GravityGameObject.h"
#include "Powerup.h"

class PowerupGameObject: public GravityGameObject
{
private:
	Powerup& dataStruct = Powerup{};

public:
	PowerupGameObject(glm::vec3& entityPosition, GLint entityNumElements);

	inline Powerup& getPowerup() { return dataStruct; }

	// Variables that give the texture IDs for the texture of the entity.
	static GLuint armorTextureID;
	static GLuint airTextureID;
	static GLuint pepperTextureID;

	// Static method to load the treasure textures.
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};

