#include "JellyfishGameObject.h"

JellyfishGameObject::JellyfishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements) :
	EnemyGameObject(entityPos, entityTexture, entityNumElements) {
	health = 25;
	scale = glm::vec3(0.8, 0.8, 1);
	boundingBox = glm::vec2(0.8, 0.8);
}