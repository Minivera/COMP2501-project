#include "TreasureGameObject.h"

TreasureGameObject::TreasureGameObject(int entityValue, glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements)
	: GravityGameObject(entityPosition, glm::vec3(0.3f, 0.3f, 1.0f), 0, entityTexture, entityNumElements) {
	value = entityValue;
}