#include "EnemyGameObject.h"

#include "WeaponGameObject.h"
#include "TreasureGameObject.h"
#include "PlayerGameObject.h"
#include "Random.h"

EnemyGameObject::EnemyGameObject(glm::vec3& entityPos, GLuint entityTexture, GLint entityNumElements):
	GameObject(entityPos, entityTexture, entityNumElements) {}

void EnemyGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	// Get the player
	shared_ptr<PlayerGameObject> player = dynamic_pointer_cast<PlayerGameObject>(entities.at(0));
	auto playerPos = player->getPosition();

	// Attemps to collide with something
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Checks if the current object collides with a weapon of some kind
		auto weapon = dynamic_pointer_cast<WeaponGameObject>(*it);
		if (weapon && checkCollision(*(*it))) {
			hurt(weapon->getDamage());
			weapon->setDirty(true);
		}
	}

	// Handle hurting mechanism
	if (currentState == EnemyState::HURT) {
		// We processed the hurt state, now we're executing it.
		currentState = EnemyState::HURTING;
	}
	else if (currentState == EnemyState::HURTING) {
		invicibilityTimer -= deltaTime;
	}

	if (currentState == EnemyState::CHASE) {
		// We process the chase state, now we're executing it.
		currentState = EnemyState::CHASING;
	}
	else if (currentState == EnemyState::CHASING) {
		// Calculate the angle between self and player
		rotation = glm::degrees(glm::atan(
			position.y - playerPos.y,
			position.x - playerPos.x
		));

		// TODO: Try to create a line of sight mecanism to prevent the fish from seeing the player through walls.
		// Move towards the player
		glm::vec3 desiredVelocity = glm::normalize(playerPos - position) * maxSpeed * float(deltaTime);
		glm::vec3 steering = desiredVelocity - velocity;
		velocity += steering;
	}
	
	if (currentState == EnemyState::DIE) {
		// Drop treasure and set as dirty
		dirty = true;
		entities.push_back(make_shared<TreasureGameObject>(random::randomInt(1, 3) * 5, position + glm::vec3(0, -0.3, 0), TreasureGameObject::treasureTextureID, numElements));
	}

	GameObject::update(entities, deltaTime);
}

// Renders the PlayerGameObject using the shader and then renders their weapon
void EnemyGameObject::render(Shader& shader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, texture);

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
	shader.setUniformMat4("transformationMatrix", transformationMatrix);
	if (currentState == EnemyState::HURTING && random::randomBool()) {
		shader.setUniform4f("objectColor", glm::vec4(1, 0, 0, 0.6));
	}
	else {
		shader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));
	}

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
}

void EnemyGameObject::clean() {
	if (health <= 0) {
		currentState = EnemyState::DIE;
	}

	if (currentState == EnemyState::HURTING && invicibilityTimer <= 0) {
		currentState = EnemyState::IDLE;
		invicibilityTimer = 0;
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

GLuint EnemyGameObject::fishTextureID;
GLuint EnemyGameObject::jellyfishTextureID;
GLuint EnemyGameObject::smokerTextureID;

int EnemyGameObject::setTextures(void (setFuncPtr)(GLuint w, char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\enemies\\fish-single.png");
	setFuncPtr(textures[offset + 1], "Assets\\enemies\\fish-big-single.png");
	setFuncPtr(textures[offset + 2], "Assets\\enemies\\mine.png");
	fishTextureID = textures[offset + 0];
	jellyfishTextureID = textures[offset + 1];
	smokerTextureID = textures[offset + 2];
	return offset + 3;
}
