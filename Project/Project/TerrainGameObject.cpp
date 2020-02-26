#include "TerrainGameObject.h"

TerrainGameObject::TerrainGameObject(TerrainType entityType, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements):
	GameObject(entityPosition, entityScale, entityRotation, entityTexture, entityNumElements) {
	type = entityType;
}

int TerrainGameObject::setTerrainTexture(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\environment\\tiles-top-1.png");
	setFuncPtr(textures[offset + 1], "Assets\\environment\\tiles-top-2.png");
	setFuncPtr(textures[offset + 2], "Assets\\environment\\tiles-bottom-1.png");
	setFuncPtr(textures[offset + 3], "Assets\\environment\\tiles-bottom-2.png");
	return offset + 4;
}
