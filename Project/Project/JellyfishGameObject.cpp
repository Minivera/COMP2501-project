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
	if (currentState != EnemyState::DIE && currentState != EnemyState::FLEEING && glm::distance(playerPos, position) <= fleeRange) {
		bool readyToFlee = true;
		// Check if we see any wall between us and the player
		for (auto it = entities.begin(); it != entities.end(); it++) {
			// If we have a wall between us and the player
			auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
			if (terrain && seesEntity(player->getPosition(), *terrain) && glm::distance(playerPos, position) > glm::distance(terrain->getPosition(), position)) {
				readyToFlee = false;
				break;
			}
		}

		if (readyToFlee && waiting()) {
			// If we are not fleeing, but should, start
			currentState = EnemyState::FLEE;
		}
		else if (!readyToFlee && currentState == EnemyState::FLEEING) {
			// If we shouldn't flee, but we are fleeing, stop.
			currentState = EnemyState::IDLE;
			velocity = glm::vec3();
		}
	}

	// If currently fleeing, stop when the player is far enough
	if (currentState == EnemyState::FLEEING && glm::distance(playerPos, position) > fleeRange) {
		currentState = EnemyState::IDLE;
		velocity = glm::vec3();
	}

	if (currentState == EnemyState::FLEE || currentState == EnemyState::FLEEING) {
		// If we are feeling, ignore all attack mechanisms
		return EnemyGameObject::update(entities, deltaTime);
	}

	bool readyToAttack = false;
	if (currentState != EnemyState::DIE && currentState != EnemyState::ATTACKING &&
		glm::distance(playerPos, position) <= viewRange) {
		readyToAttack = true;
		// Check if we see any wall btween us and the player
		for (auto it = entities.begin(); it != entities.end(); it++) {
			// If we have a wall between us and the player
			auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
			if (terrain && seesEntity(player->getPosition(), *terrain) && glm::distance(playerPos, position) > glm::distance(terrain->getPosition(), position)) {
				readyToAttack = false;
				break;
			}
		}

		if (readyToAttack && waiting()) {
			// If we are not attacking, but should, start
			currentState = EnemyState::ATTACK;
		}
		else if (!readyToAttack && currentState == EnemyState::ATTACKING) {
			// If we shouldn't attack, but we are attacking, stop.
			currentState = EnemyState::IDLE;
			velocity = glm::vec3();
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

	GLfloat attackAngle = glm::degrees(glm::atan(
		playerPos.y - position.y,
		playerPos.x - position.x
	));

	// Add an electricity game object to the vector of game objects
	entities.push_back(make_shared<ElectricityGameObject>(
		attackRange,
		glm::vec3(attackSpeed, 0, 0),
		position,
		attackAngle,
		ElectricityGameObject::projectileTextureID,
		6
	));
}