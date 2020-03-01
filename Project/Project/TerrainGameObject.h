#pragma once

#include "GameObject.h"

enum class TerrainType { Floor, Wall, Ceilling };

class TerrainGameObject: public GameObject {
private:
	TerrainType type;
public:
	TerrainGameObject(TerrainType entityType, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements);

	inline TerrainType getType() { return type; }

	// Variables that give the texture IDs for the texture of the entity
	static GLuint ceiling1TextureID;
	static GLuint ceiling2TextureID;
	static GLuint floor1TextureID;
	static GLuint floor2TextureID;
	static GLuint wallLeftTextureID;
	static GLuint wallRightTextureID;
	static GLuint slantTopLeftTextureID;
	static GLuint slantTopRightTextureID;
	static GLuint slantBottomLeftTextureID;
	static GLuint slantBottomRightTextureID;

	// Static method to load the terrain features from the sprite sheet
	// Returns the number of loaded textures
	static int setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset);
};