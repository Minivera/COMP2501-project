#include "TreasureGameObject.h"

TreasureGameObject::TreasureGameObject(int entityValue, glm::vec3& entityPosition, glm::vec3& entityScale, GLuint entityTexture, GLint entityNumElements)
	: GravityGameObject(entityPosition, entityScale, 0, entityTexture, entityNumElements) {
	value = entityValue;
}