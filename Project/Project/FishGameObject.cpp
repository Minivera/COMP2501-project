#include "FishGameObject.h"

#include "PlayerGameObject.h"

FishGameObject::FishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements):
	EnemyGameObject(entityPos, entityTexture, entityNumElements) {
	health = 15;
	scale = glm::vec3(0.5, 0.5, 1);
}

void FishGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// Get the player
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	auto playerPos = player->getPosition();

	if (glm::distance(playerPos, position) < viewRange && currentState == EnemyState::IDLE) {
		// TODO: Try to create a line of sight mecanism to prevent the fish from seeing the player through walls.
		currentState = EnemyState::CHASE;
	}
	else if (glm::distance(playerPos, position) >= viewRange && currentState != EnemyState::DIE) {
		currentState = EnemyState::IDLE;
	}

	EnemyGameObject::update(entities, deltaTime);
}