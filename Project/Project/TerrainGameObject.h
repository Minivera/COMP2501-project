#pragma once

#include "GameObject.h"

enum class TerrainType { Floor, Wall, Ceilling };

class TerrainGameObject: public GameObject {
private:
	TerrainType type;
public:
	TerrainGameObject(TerrainType entityType, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);

	// Static method to load the terrain features from the sprite sheet
	// Returns the number of loaded textures
	static int setTerrainTexture(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);

	inline TerrainType getType() { return type; }
};