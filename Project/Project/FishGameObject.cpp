#include "FishGameObject.h"

#include "PlayerGameObject.h"
#include "TerrainGameObject.h"

FishGameObject::FishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements):
	EnemyGameObject(entityPos, entityTexture, entityNumElements) {
	health = 15;
	scale = glm::vec3(0.5, 0.5, 1);
	boundingBox = glm::vec2(0.5, 0.5);
}

void FishGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// Get the player
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	auto playerPos = player->getPosition();

	bool chasing = false;
 	if (currentState != EnemyState::DIE && currentState == EnemyState::IDLE && currentState != EnemyState::CHASING &&
		seesEntity(viewRange, *player)) {
		chasing = true;
		// Check if we see any wall btween us and the player
		for (auto it = entities.begin(); it != entities.end(); it++) {
			// If we have a wall between us and the player
			auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
			if (terrain && seesEntity(viewRange, *terrain) && glm::distance(playerPos, position) > glm::distance(terrain->getPosition(), position)) {
				chasing = false;
				break;
			}
		}

		if (chasing) {
 			currentState = EnemyState::CHASE;
		}
		else {
			currentState = EnemyState::IDLE;
			velocity = glm::vec3();
		}
	}

	EnemyGameObject::update(entities, deltaTime);
}