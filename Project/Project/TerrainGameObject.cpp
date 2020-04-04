#include "TerrainGameObject.h"

TerrainGameObject::TerrainGameObject(TerrainType entityType, glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements):
	GameObject(entityPosition, entityScale, entityRotation, entityTexture, entityNumElements) {
	type = entityType;
}

GLuint TerrainGameObject::bgTextureID = 0;
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

int TerrainGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\environment\\tiles-bg.png");
	setFuncPtr(textures[offset + 1], "Assets\\environment\\tiles-top-1.png");
	setFuncPtr(textures[offset + 2], "Assets\\environment\\tiles-top-2.png");
	setFuncPtr(textures[offset + 3], "Assets\\environment\\tiles-bottom-1.png");
	setFuncPtr(textures[offset + 4], "Assets\\environment\\tiles-bottom-2.png");
	setFuncPtr(textures[offset + 5], "Assets\\environment\\tiles-left.png");
	setFuncPtr(textures[offset + 6], "Assets\\environment\\tiles-right.png");
	setFuncPtr(textures[offset + 7], "Assets\\environment\\slant-top-left.png");
	setFuncPtr(textures[offset + 8], "Assets\\environment\\slant-top-right.png");
	setFuncPtr(textures[offset + 9], "Assets\\environment\\slant-bottom-left.png");
	setFuncPtr(textures[offset + 10], "Assets\\environment\\slant-bottom-right.png");
	bgTextureID = textures[offset + 0];
	ceiling1TextureID = textures[offset + 1];
	ceiling2TextureID = textures[offset + 2];
	floor1TextureID = textures[offset + 3];
	floor2TextureID = textures[offset + 4];
	wallLeftTextureID = textures[offset + 5];
	wallRightTextureID = textures[offset + 6];
	slantTopLeftTextureID = textures[offset + 7];
	slantTopRightTextureID = textures[offset + 8];
	slantBottomLeftTextureID = textures[offset + 9];
	slantBottomRightTextureID = textures[offset + 10];
	return offset + 11;
}
