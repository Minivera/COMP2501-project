#include "TreasureGameObject.h"

TreasureGameObject::TreasureGameObject(int entityValue, glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements)
	: GravityGameObject(entityPosition, glm::vec3(0.3f, 0.3f, 1.0f), 0, entityTexture, entityNumElements) {
	value = entityValue;
}

GLuint TreasureGameObject::treasureTextureID = 0;

int TreasureGameObject::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\objects\\treasure-big.png");
	treasureTextureID = textures[offset + 0];
	return offset + 1;
}