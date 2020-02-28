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

	if (glm::distance(playerPos, position) < viewRange) {
		// Calculate the angle between self and player
		angle = rotation = glm::degrees(glm::atan(
			position.y - playerPos.y,
			position.x - playerPos.x
		));

		// TODO: Try to create a line of sight mecanism to prevent the fish from seeing the player through walls.
	}
	else {
		currentState = EnemyState::IDLE;
	}

	EnemyGameObject::update(entities, deltaTime);
}