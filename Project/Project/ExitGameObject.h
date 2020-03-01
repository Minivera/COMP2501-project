#pragma once

#include "GameObject.h"

class ExitGameObject: public GameObject
{
public:
	ExitGameObject(glm::vec3& entityPos, GLuint texture, GLint entityNumElements);

	// Variables that give the texture IDs for the texture of the entity.
	static GLuint exitTextureID;

	// Static method to load the various enemy sprites from the filesystem
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};

