#include <iostream>

#include "GravityGameObject.h"
#include "TerrainGameObject.h"

GravityGameObject::GravityGameObject(glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityTexture, entityNumElements) {}

GravityGameObject::GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityScale, entityRotation, entityTexture, entityNumElements) {}

GravityGameObject::GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityVelocity, entityScale, entityRotation, entityTexture, entityNumElements) {}

void GravityGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	if (!gravityActivated) {
		// If we deactivated gravity, stop it from affecting the entity
		acceleration.y = 0;
		return GameObject::update(entities, deltaTime);
	}

	bool collidesWithFloor = false;

	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a floor or with a top slant
		auto converted = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (converted && checkCollision(*(*it)) && (converted->getType() == TerrainType::Floor || converted->getType() == TerrainType::BottomSlant)) {
			collidesWithFloor = true;
			break;
		}
	}

	if (collidesWithFloor) {
		// If we have a collision, stop gravity
		acceleration.y = 0;
	}
	else {
		// If we don't have any collision, apply gravity
		acceleration += glm::vec3(0.0f, gravityAccel, 0.0f);
	}

	GameObject::update(entities, deltaTime);
}