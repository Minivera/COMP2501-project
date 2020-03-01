#include "TerrainGameObject.h"

TerrainGameObject::TerrainGameObject(TerrainType entityType, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements):
	GameObject(entityPosition, entityScale, entityRotation, entityTexture, entityNumElements) {
	type = entityType;
}

GLuint TerrainGameObject::ceiling1TextureID = 0;
GLuint TerrainGameObject::ceiling2TextureID = 0;
GLuint TerrainGameObject::floor1TextureID = 0;
GLuint TerrainGameObject::floor2TextureID = 0;
GLuint TerrainGameObject::wallLeftTextureID = 0;
GLuint TerrainGameObject::wallRightTextureID = 0;
GLuint TerrainGameObject::slantTopLeftTextureID = 0;
GLuint TerrainGameObject::slantTopRightTextureID = 0;
GLuint TerrainGameObject::slantBottomLeftTextureID = 0;
GLuint TerrainGameObject::slantBottomRightTextureID = 0;

int TerrainGameObject::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\environment\\tiles-top-1.png");
	setFuncPtr(textures[offset + 1], "Assets\\environment\\tiles-top-2.png");
	setFuncPtr(textures[offset + 2], "Assets\\environment\\tiles-bottom-1.png");
	setFuncPtr(textures[offset + 3], "Assets\\environment\\tiles-bottom-2.png");
	setFuncPtr(textures[offset + 4], "Assets\\environment\\tiles-left.png");
	setFuncPtr(textures[offset + 5], "Assets\\environment\\tiles-right.png");
	setFuncPtr(textures[offset + 6], "Assets\\environment\\slant-top-left.png");
	setFuncPtr(textures[offset + 7], "Assets\\environment\\slant-top-right.png");
	setFuncPtr(textures[offset + 8], "Assets\\environment\\slant-bottom-left.png");
	setFuncPtr(textures[offset + 9], "Assets\\environment\\slant-bottom-right.png");
	ceiling1TextureID = textures[offset + 0];
	ceiling2TextureID = textures[offset + 1];
	floor1TextureID = textures[offset + 2];
	floor2TextureID = textures[offset + 3];
	wallLeftTextureID = textures[offset + 4];
	wallRightTextureID = textures[offset + 5];
	slantTopLeftTextureID = textures[offset + 6];
	slantTopRightTextureID = textures[offset + 7];
	slantBottomLeftTextureID = textures[offset + 8];
	slantBottomRightTextureID = textures[offset + 9];
	return offset + 10;
}
