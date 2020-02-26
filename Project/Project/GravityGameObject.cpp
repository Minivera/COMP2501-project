#include <iostream>

#include "GravityGameObject.h"
#include "TerrainGameObject.h"

GravityGameObject::GravityGameObject(glm::vec3& entityPosition, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityTexture, entityNumElements) {}

GravityGameObject::GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityScale, entityRotation, entityTexture, entityNumElements) {}

GravityGameObject::GravityGameObject(glm::vec3& entityPosition, glm::vec3& entityVelocity, glm::vec3& entityScale, GLfloat entityRotation, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPosition, entityVelocity, entityScale, entityRotation, entityTexture, entityNumElements) {}

void GravityGameObject::update(std::vector<shared_ptr<GameObject>> entities, double deltaTime) {
	bool collidesWithFloor = false;

	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a wall
		auto converted = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (converted && checkCollision(*(*it)) && converted->getType() == TerrainType::Floor) {
			collidesWithFloor = true;
			break;
		}
	}

	if (gravityActivated && !collidesWithFloor) {
		velocity += glm::vec3(0, -gravityAccel * deltaTime, 0);
	}

	// Still stop any downward movement if we happened to collide with an object
	if (collidesWithFloor && velocity.y < 0) {
		velocity.y = 0;
	}

	GameObject::update(entities, deltaTime);
}