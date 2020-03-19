#include "JellyfishGameObject.h"

#include "PlayerGameObject.h"
#include "TerrainGameObject.h"
#include "ElectricityGameObject.h"

JellyfishGameObject::JellyfishGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements) :
	EnemyGameObject(entityPos, entityTexture, entityNumElements) {
	health = 25;
	scale = glm::vec3(0.8, 0.8, 1);
	boundingBox = glm::vec2(0.8, 0.8);
}

void JellyfishGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// Get the player
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	auto playerPos = player->getPosition();

	// Check the distance between ourselves and the player. If the player is too close, try to flee.
	/*if (currentState != EnemyState::DIE && currentState != EnemyState::FLEEING && glm::distance(playerPos, position) <= fleeRange) {
		currentState = EnemyState::FLEE;
  		return EnemyGameObject::update(entities, deltaTime);
	}*/

	// If currently fleeing, stop when the player is far enough
	if (currentState == EnemyState::FLEEING && glm::distance(playerPos, position) > fleeRange) {
		currentState = EnemyState::IDLE;
	}

	bool readyToAttack = false;
	if (currentState != EnemyState::DIE && currentState == EnemyState::IDLE && currentState != EnemyState::ATTACKING &&
		seesEntity(viewRange, *player)) {
		readyToAttack = true;
		// Check if we see any wall btween us and the player
		for (auto it = entities.begin(); it != entities.end(); it++) {
			// If we have a wall between us and the player
			auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
			if (terrain && seesEntity(viewRange, *terrain) && glm::distance(playerPos, position) > glm::distance(terrain->getPosition(), position)) {
				readyToAttack = false;
				break;
			}
		}

		if (readyToAttack) {
			currentState = EnemyState::ATTACK;
		}
		else {
			currentState = EnemyState::IDLE;
		}
	}

	if (currentState == EnemyState::ATTACK) {
		// If we received an attack signal, process it
		attackTimer = attackWaitTime;
		currentState = EnemyState::ATTACKING;
		attack(entities);
	}
	if (currentState == EnemyState::ATTACKING) {
		// Wait while we're still attacking.
		attackTimer -= deltaTime;
	}

	EnemyGameObject::update(entities, deltaTime);
}

void JellyfishGameObject::clean() {
	// If still attacking and out attack timer got to zero
	if (currentState == EnemyState::ATTACKING && attackTimer <= 0) {
		// Go back to IDLE and prepare a new attack in update.
		attackTimer = 0;
		currentState = EnemyState::IDLE;
	}

	EnemyGameObject::clean();
}

void JellyfishGameObject::attack(std::vector<shared_ptr<GameObject>>& entities) {
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	auto playerPos = player->getPosition();

	rotation = glm::degrees(glm::atan(
		position.y - playerPos.y,
		position.x - playerPos.x
	));

	// Add an elecriticty game object to the vector of game objects
	entities.push_back(make_shared<ElectricityGameObject>(
		attackWaitTime,
		glm::vec3(attackSpeed, 0, 0),
		position,
		rotation,
		ElectricityGameObject::projectileTextureID,
		6
	));
}