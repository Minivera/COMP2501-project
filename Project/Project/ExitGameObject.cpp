#include "ExitGameObject.h"

ExitGameObject::ExitGameObject(glm::vec3& entityPos, GLuint texture, GLint entityNumElements):
	GameObject(entityPos, glm::vec3(2.0f, 1.0f, 1.0f), 0, texture, entityNumElements) {}

GLuint ExitGameObject::exitTextureID = 0;

int ExitGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\environment\\exit.png");
	exitTextureID = textures[offset + 0];
	return offset + 1;
}