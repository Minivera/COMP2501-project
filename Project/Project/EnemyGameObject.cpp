#include "EnemyGameObject.h"

#include "WeaponGameObject.h"
#include "TreasureGameObject.h"
#include "PowerupGameObject.h"
#include "PlayerGameObject.h"
#include "TerrainGameObject.h"
#include "Random.h"
#include "LineOfSight.h"

EnemyGameObject::EnemyGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements) :
	GameObject(entityPos, entityTexture, entityNumElements) {}

bool EnemyGameObject::seesEntity(const glm::vec3& direction, const GameObject& other) {
	glm::vec3 selfPos = position;
	glm::vec3 otherPos = other.getPosition();
	glm::vec2 otherBoundingBox = other.getBoundingBox();

	auto lineOfSight = LineOfSight::drawLine(selfPos.x, selfPos.y, direction.x, direction.y);
	auto entityRectangle = LineOfSight::drawRectangle(
		otherPos.x - otherBoundingBox.x / 2, 
		otherPos.y + otherBoundingBox.y / 2,
		otherPos.x + otherBoundingBox.x / 2,
		otherPos.y - otherBoundingBox.y / 2
	);

	return LineOfSight::intersectsWith(lineOfSight, entityRectangle);
}

void EnemyGameObject::preventCollisions(std::vector<shared_ptr<GameObject>>& entities) {
	shared_ptr<TerrainGameObject> collidesWith;

	// Attemps to collide with something
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a wall
		auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (terrain && checkCollision(*(*it))) {
			collidesWith = terrain;
			break;
		}
	}

	// If colloding with a wall, process which side has been collided with
	if (collidesWith != nullptr && (collidesWith->getType() == TerrainType::Wall ||
		collidesWith->getType() == TerrainType::BottomSlant || collidesWith->getType() == TerrainType::TopSlant)) {
		// Prevent movement on the side we collided with the wall
		velocity.y = 0;
		velocity.x = 0;
	}
	else if (collidesWith != nullptr && (collidesWith->getType() == TerrainType::Floor || collidesWith->getType() == TerrainType::BottomSlant)) {
		// If colliding with the floor or a bottom facing slant, stop downward movement
		velocity.y = velocity.y < 0 ? 0 : velocity.y;
	}
	else if (collidesWith != nullptr && (collidesWith->getType() == TerrainType::Ceilling || collidesWith->getType() == TerrainType::TopSlant)) {
		// If colliding with the ceiling or a top facing slant, stop upward movement
		velocity.y = velocity.y > 0 ? 0 : velocity.y;
	}
}

void EnemyGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	entityTime += (float)deltaTime;

	// Get the player
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	auto playerPos = player->getPosition();

	// Attemps to collide with something
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a weapon of some kind
		auto weapon = dynamic_pointer_cast<WeaponGameObject>(*it);
		if (currentState != EnemyState::DIE && currentState != EnemyState::HURT && currentState != EnemyState::HURTING && weapon && checkCollision(*(*it))) {
			hurt(weapon->getDamage());
			weapon->setDirty(true);
			angleToCollision = weapon->getAngle();
			break;
		}
	}

	// Handle hurting mechanism
	if (currentState == EnemyState::HURT) {
		// We processed the hurt state, now we're executing it.
		currentState = EnemyState::HURTING;
	}
	else if (currentState == EnemyState::HURTING) {
		invicibilityTimer -= deltaTime;

		angle = angleToCollision;
		velocity = glm::vec3(1, 0, 0);
	}

	// Handle collision with player mechanism
	if (currentState == EnemyState::COLLIDE) {
		// We processed the collide state, now we're executing it.
		currentState = EnemyState::COLLIDING;
	}
	else if (currentState == EnemyState::COLLIDING) {
		collisonTimer -= deltaTime;

		angle = angleToCollision;
		velocity = glm::vec3(0.2, 0, 0);
	}

	if (currentState == EnemyState::CHASE) {
		// We processed the chase state, now we're executing it.
		currentState = EnemyState::CHASING;
	}
	else if (currentState == EnemyState::CHASING) {
		// Calculate the angle between self and player
		rotation = glm::degrees(glm::atan(
			position.y - playerPos.y,
			position.x - playerPos.x
		));

		// Move towards the player
		glm::vec3 desiredVelocity = glm::normalize(playerPos - position) * maxSpeed;
		glm::vec3 steering = desiredVelocity - velocity;
		velocity += steering;
	}

	if (currentState == EnemyState::FLEE) {
		// We processed the flee state, now we're executing it.
		currentState = EnemyState::FLEEING;
	}
	else if (currentState == EnemyState::FLEEING) {
		// Calculate the angle between self and player
		rotation = glm::degrees(glm::atan(
			position.y - playerPos.y,
			position.x - playerPos.x
		)) + 180;

		// Move away the player
		glm::vec3 desiredVelocity = glm::normalize(playerPos - position) * maxSpeed;
		glm::vec3 steering = -desiredVelocity - velocity;
		velocity += steering;
	}

	if (currentState == EnemyState::WANDER) {
		// We processed the wander state, now we're executing it.
		currentState = EnemyState::WANDERING;
	}
	else if (currentState == EnemyState::WANDERING) {
		// Update the current path based on the position
		currentPath->update(position);

		// If the path is complete, proceed to generate another one
		if (currentPath->complete()) {
			currentPath = levelWorld->randomizedPathfind(currentPath->getObjective(), pathfindDistance);
		}

		// Start chasing the path
		glm::vec3 desiredVelocity = glm::normalize(currentPath->currentNode()->getPosition() - position);
		velocity = glm::vec3(desiredVelocity.x * maxSpeed, desiredVelocity.y * maxSpeed, 0.0f);
		rotation = glm::degrees(glm::atan(desiredVelocity.x, desiredVelocity.y)) + 90.0f;
	}
	
	if (currentState == EnemyState::DIE) {
		// Drop treasure or powerup and set as dirty
		dirty = true;
		if (random::randomFloat(0.0f, 1.0f) < 0.65f) {
			// 65% change to drop treasure
			entities.push_back(make_shared<TreasureGameObject>(random::randomInt(1, 3) * 5, position + glm::vec3(0, -0.3, 0), TreasureGameObject::treasureTextureID, numElements));
		}
		else {
			entities.push_back(make_shared<PowerupGameObject>(position + glm::vec3(0, -0.3, 0), numElements));
		}
	}

	preventCollisions(entities);

	// If we're idle, try wandering
	if (currentState == EnemyState::IDLE) {
		currentState = EnemyState::WANDER;
	}

	GameObject::update(entities, deltaTime);
}

// Renders the PlayerGameObject using the shader and then renders their weapon
void EnemyGameObject::render(Shader& spriteShader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);
	
	spriteShader.setUniform1f("count", 4.0f);
	spriteShader.setUniform1f("time", entityTime);

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a rotation matrix to rotate the sprite depending on the angle
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0, 0, 1.0f));
	transformationMatrix = glm::rotate(transformationMatrix, 180.0f, glm::vec3(0, 1, 0));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	if (currentState == EnemyState::HURTING && random::randomBool()) {
		spriteShader.setUniform4f("objectColor", glm::vec4(1, 0, 0, 0.6));
	}
	else {
		spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	}

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	// Reset for other sprites
	spriteShader.setUniform1f("count", 0);
}

void EnemyGameObject::clean() {
	if (health <= 0) {
		currentState = EnemyState::DIE;
	}

	if (currentState == EnemyState::HURTING && invicibilityTimer <= 0) {
		currentState = EnemyState::IDLE;
		invicibilityTimer = 0;
		angle = 0;
		velocity = glm::vec3();
	}

	if (currentState == EnemyState::COLLIDING && collisonTimer <= 0) {
		currentState = EnemyState::IDLE;
		collisonTimer = 0;
		angle = 0;
		velocity = glm::vec3();
	}
}

void EnemyGameObject::attack() {
	if (currentState != EnemyState::ATTACKING && currentState != EnemyState::DIE) {
		currentState = EnemyState::ATTACK;
	}
}

void EnemyGameObject::hurt(int damage) {
	if (currentState != EnemyState::DIE && currentState != EnemyState::HURT && currentState != EnemyState::HURTING) {
		health -= damage;
		invicibilityTimer = invicibleTime;
		currentState = EnemyState::HURT;
	}
}

void EnemyGameObject::collide() {
	if (currentState != EnemyState::DIE && currentState != EnemyState::COLLIDE && currentState != EnemyState::COLLIDING) {
		collisonTimer = collisionTime;
		currentState = EnemyState::COLLIDE;
		angleToCollision -= glm::degrees(glm::atan(velocity.x, velocity.y));
	}
}

void EnemyGameObject::setWorld(shared_ptr<GameWorld> world) {
	this->levelWorld = world;

	currentPath = levelWorld->randomizedPathfind(levelWorld->closestNodeToPosition(position), pathfindDistance);
}

GLuint EnemyGameObject::fishTextureID;
GLuint EnemyGameObject::jellyfishTextureID;
GLuint EnemyGameObject::smokerTextureID;
GLuint EnemyGameObject::smokerFogTextureID;

int EnemyGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\enemies\\fish.png");
	setFuncPtr(textures[offset + 1], "Assets\\enemies\\fish-big.png");
	setFuncPtr(textures[offset + 2], "Assets\\enemies\\smoker.png");
	setFuncPtr(textures[offset + 3], "Assets\\FX\\enemy-death.png");
	fishTextureID = textures[offset + 0];
	jellyfishTextureID = textures[offset + 1];
	smokerTextureID = textures[offset + 2];
	smokerFogTextureID = textures[offset + 3];
	return offset + 4;
}
