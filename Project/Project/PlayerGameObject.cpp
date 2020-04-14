#include "PlayerGameObject.h"

#include "TerrainGameObject.h"
#include "TreasureGameObject.h"
#include "EnemyGameObject.h"
#include "Window.h"
#include "Random.h"
#include "LineOfSight.h"

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(glm::vec3 &entityPos, GLuint entityTexture, GLuint entityHarpoonTexture, GLuint entityPistolTexture, 
	GLuint entityLaserTexture, GLuint entityBulletTexture, GLuint entityBubblesTexture, GLint entityNumElements)
	: GravityGameObject(entityPos, entityTexture, entityNumElements) {
	harpoonTexture = entityHarpoonTexture;
	pistolTexture = entityPistolTexture;
	laserTexture = entityLaserTexture;
	bubblesTexture = entityBubblesTexture;
	boundingBox = glm::vec2(0.4, 0.6);

	inventory = make_unique<PlayerInventory>(entityBulletTexture, entityNumElements);
}

void PlayerGameObject::update(std::vector<shared_ptr<GameObject>>& entities, double deltaTime) {
	playerTime += (float)deltaTime;

	inventory->update(deltaTime);

	// Reload the current weapon from the inventory
	inventory->getEquipedWeapon().recharge(deltaTime);

	shared_ptr<TerrainGameObject> collidesWith;

	float maxDistance = glm::max(scale.x, scale.y) * 2;
	// Attemps to collide with something
	for (auto it = entities.begin(); it != entities.end(); it++) {
		// Don't check anything if the entity is beyond the reach of the player
		if (glm::distance(position, (*it)->getPosition()) > maxDistance) {
			continue;
		}

		// Checks if the current object collides with a wall
		auto terrain = dynamic_pointer_cast<TerrainGameObject>(*it);
		if (terrain && terrain->getType() != TerrainType::BG && checkCollision(*(*it))) {
			collidesWith = terrain;
		}

		// Check if player is near some treasure they can pick up
		auto treasure = dynamic_pointer_cast<TreasureGameObject>(*it);
		if (currentState != PlayerState::HURTING && treasure && checkCollision(*(*it))) {
			treasure->setDirty(true);
			pickUp(treasure);
		}

		// Check if player is near some powerup they can pick up
		auto powerup = dynamic_pointer_cast<PowerupGameObject>(*it);
		if (currentState != PlayerState::HURTING && powerup && checkCollision(*(*it))) {
			powerup->setDirty(true);
			pickUp(powerup);
		}

		// Check if the player is getting hit by something
		auto enemy = dynamic_pointer_cast<EnemyGameObject>(*it);
		if (currentState != PlayerState::HURT && currentState != PlayerState::HURTING && enemy && checkCollision(*(*it))) {
			enemy->collide();
			hurt(*enemy);
		}
	}
	
	// If colloding with a wall, process which side has been collided with
	if (collidesWith != nullptr && (collidesWith->getType() == TerrainType::Wall ||
		collidesWith->getType() == TerrainType::BottomSlant || collidesWith->getType() == TerrainType::TopSlant)) {
		// If colliding with a wall, deactivate gravity
		gravityActivated = false;

		// Prevent movement on the side we collided with the wall
		auto collisionSide = getCollisionSide(*collidesWith);
		if ((velocity.x > 0 && collisionSide == CollisionSides::Left) || (velocity.x < 0 && collisionSide == CollisionSides::Right)) {
			velocity.y = acceleration.y = 0;
			velocity.x = acceleration.x = 0;
		}
	}
	else if (collidesWith != nullptr && (collidesWith->getType() == TerrainType::Floor || collidesWith->getType() == TerrainType::BottomSlant)) {
		// If colliding with the floor or a bottom facing slant, slow down movement
		velocity.y = acceleration.y < 0 ? 0 : velocity.y;
		velocity.x *= wallSlowEffect;
	}
	else if (collidesWith != nullptr && (collidesWith->getType() == TerrainType::Ceilling || collidesWith->getType() == TerrainType::TopSlant)) {
		// If colliding with the ceiling or a top facing slant, slow down movement and stop lift
		velocity.y = acceleration.y = 0;
		velocity.x *= wallSlowEffect;
	}

	// Check if we are moving and have a forward velocity, apply friction
	if (velocity.x != 0 && !moving) {
		acceleration += glm::vec3(-velocity.x, 0.0f, 0.0f);
	}
	else {
		acceleration.x = 0;
	}

	// Handle attack mechanism
	if (currentState == PlayerState::ATTACK) {
		// We processed the attack state, now we're executing it.
		currentState = PlayerState::ATTACKING;
		inventory->getEquipedWeapon().attack(position, armRotation, inventory->damageBoost(), entities);

		armMovement = inventory->getEquipedWeapon().getRechargeRate();
	}
	else if (currentState == PlayerState::ATTACKING) {
		armMovement -= deltaTime;
		if (armMovement <= 0) {
			armMovement = 0;
			currentState = PlayerState::NONE;
		}
	}

	// Handle hurting mechanism if not invulnerable
	if (currentState == PlayerState::HURT) {
		// We processed the hurt state, now we're executing it.
		currentState = PlayerState::HURTING;

		invicibilityTimer = invicibleTime;

		// Lose some treasure when hurt
		int treasureLoss = inventory->loseTreasure();

		// lose some air when hurt
		inventory->loseAir();

		// Drop treasure at current position
		entities.push_back(make_shared<TreasureGameObject>(treasureLoss, position + glm::vec3(0, -0.3, 0), TreasureGameObject::treasureTextureID, numElements));
	} else if (currentState == PlayerState::HURTING) {
		invicibilityTimer -= deltaTime;

		velocity = -collidedVelicity;
	}

	// If we're accelerating more than the velocity, then we should stop
	glm::vec3 change = velocity * (float)deltaTime + acceleration * 0.5f * (float)deltaTime * (float)deltaTime;
	if ((velocity.x > 0 && change.x < 0) || (velocity.x < 0 && change.x > 0)) {
		velocity.x = acceleration.x = 0;
	}

	GravityGameObject::update(entities, deltaTime);
}

// Renders the PlayerGameObject using the shader and then renders their weapon
void PlayerGameObject::render(Shader& spriteShader) {
	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Then undo part of the offset for the orbit
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5, -0.5, 0));
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5, 0.5, 0));

	// Added a rotation matrix to rotate the sprite depending on the angle
	transformationMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0, 0, 1.0f));

	GLfloat weaponFlip = -90;
	// Flip the player when aiming on the left side
	if (armRotation > 90 || armRotation < -90) {
		transformationMatrix = glm::rotate(transformationMatrix, 180.f, glm::vec3(0, 1.0f, 0));
		// Flip everything so the weapon rotates properly as well
		weaponFlip = -weaponFlip;
		armRotation = -armRotation;
	}

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, scale);

	glm::mat4 weaponMatrix;
	switch (inventory->getEquipedWeaponType()) {
	case WeaponType::Harpoon:
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, harpoonTexture);

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(transformationMatrix, glm::vec3(0, 0, 0));

		// Then undo part of the offset for the orbit
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(-0.5, -0.5, 0));
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0.5, 0.5, 0));

		// Added a rotation matrix to rotate the sprite depending on the angle
		weaponMatrix = glm::rotate(weaponMatrix, armRotation + weaponFlip, glm::vec3(0, 0, 1.0f));

		// Added a scale matrix to scale the sprite so it can be rescaled
		weaponMatrix = glm::scale(weaponMatrix, glm::vec3(0.5, 0.75, 1));

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, armMovement, 0));
		break;
	case WeaponType::Pistol:
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, pistolTexture);

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(transformationMatrix, glm::vec3(0.2, 0, 0));

		// Then undo part of the offset for the orbit
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(-0.2, 0, 0));
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0.2, 0, 0));

		// Added a rotation matrix to rotate the sprite depending on the angle
		weaponMatrix = glm::rotate(weaponMatrix, armRotation + weaponFlip, glm::vec3(0, 0, 1.0f));

		// Bring back the offset after rotation
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, 0.1f, 0));

		// Added a scale matrix to scale the sprite so it can be rescaled
		weaponMatrix = glm::scale(weaponMatrix, glm::vec3(0.15, 0.30, 1));

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, -armMovement, 0));
		break;
	case WeaponType::Laser:
		// Bind the entities texture
		glBindTexture(GL_TEXTURE_2D, laserTexture);

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(transformationMatrix, glm::vec3(0.2, 0, 0));

		// Then undo part of the offset for the orbit
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(-0.2, 0, 0));
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0.2, 0, 0));

		// Added a rotation matrix to rotate the sprite depending on the angle
		weaponMatrix = glm::rotate(weaponMatrix, armRotation + weaponFlip, glm::vec3(0, 0, 1.0f));

		// Bring back the offset after rotation
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, 0.1f, 0));

		// Added a scale matrix to scale the sprite so it can be rescaled
		weaponMatrix = glm::scale(weaponMatrix, glm::vec3(0.30, 0.60, 1));

		// Move the weapon to the center of the player
		weaponMatrix = glm::translate(weaponMatrix, glm::vec3(0, -armMovement * 0.25, 0));
		break;
	}

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", weaponMatrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	// Bind the entities texture
	if (currentState == PlayerState::HURTING) {
		glBindTexture(GL_TEXTURE_2D, playerHurtTextureID);
		spriteShader.setUniform1f("count", 5.0f);
	}
	else if (moving) {
		glBindTexture(GL_TEXTURE_2D, playerMovingTextureID);
		spriteShader.setUniform1f("count", 7.0f);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texture);
		spriteShader.setUniform1f("count", 6.0f);
	}

	spriteShader.setUniform1f("time", playerTime);

	// Set the transformation matrix in the shader
	spriteShader.setUniformMat4("transformationMatrix", transformationMatrix);
	spriteShader.setUniform4f("objectColor", glm::vec4(0, 0, 0, 0));

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

	// Reset for other sprites
	spriteShader.setUniform1f("count", 0);
}

void PlayerGameObject::renderParticles(Shader& particlesShader) {
	// Bind the entities texture
	glBindTexture(GL_TEXTURE_2D, bubblesTexture);

	// Setup the transformation matrix for the shader
	// Start by moving to the position
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(), position);

	// Move forward so the particles are rendered above everything
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0, 0, -1.0f));

	// Move above the player
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.1f, 0.4f, 0));

	// Added a scale matrix to scale the sprite so it can be rescaled
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.3f, 0.3f, 1.0f));

	particlesShader.setUniform1f("count", 4.0f);
	particlesShader.setUniform1f("time", playerTime);
	particlesShader.setUniform1f("distance", 1.5f);

	// Set the transformation matrix in the shader
	particlesShader.setUniformMat4("transformationMatrix", transformationMatrix);

	int count = random::randomInt(1, 3);
	count = currentState == PlayerState::HURTING ? count * 4 : count;

	// Draw the entity
	glDrawElements(GL_TRIANGLES, numElements * count, GL_UNSIGNED_INT, 0);
}

void PlayerGameObject::clean() {
	gravityActivated = true;
	moving = false;

	if (currentState == PlayerState::HURTING && invicibilityTimer <= 0) {
		currentState = PlayerState::NONE;
		invicibilityTimer = 0;
	}
}

void PlayerGameObject::addLiftAcceleration() {
	acceleration.y = 0;
	moving = true;
	velocity = glm::vec3(velocity.x, glm::min(velocity.y + baseAcceleration, maxSpeed), velocity.z);
}

void PlayerGameObject::addLeftAcceleration() {
	moving = true;
	velocity = glm::vec3(glm::max(velocity.x - baseAcceleration, -(maxSpeed)), velocity.y, velocity.z);
}

void PlayerGameObject::addRightAcceleration() {
	moving = true;
	velocity = glm::vec3(glm::min(velocity.x + baseAcceleration, maxSpeed), velocity.y, velocity.z);
}

void PlayerGameObject::changeWeapon(WeaponType type) {
	if (currentState == PlayerState::NONE) {
		inventory->equip(type);
	}
}

bool PlayerGameObject::unlock(WeaponType type) {
	return inventory->unlock(type);
}

bool PlayerGameObject::upgrade(UpgradeType type) {
	if (type == UpgradeType::Flippers) {
		if (inventory->upgrade(type)) {
			maxSpeed += speedIncrease;
			return true;
		}
		return false;
	}
	return inventory->upgrade(type);
}

void PlayerGameObject::attack() {
	if (currentState == PlayerState::NONE) {
		currentState = PlayerState::ATTACK;
	}
}

void PlayerGameObject::hurt(GameObject& enemy) {
	if (!inventory->isInvulnerable() && currentState != PlayerState::HURT && currentState != PlayerState::HURTING) {
		currentState = PlayerState::HURT;

		GLfloat enemyAngle = LineOfSight::drawLine(enemy.getPosition().x, enemy.getPosition().y, position.x, position.y).angle();
		collidedVelicity = -glm::vec3(
			cos(glm::radians(enemyAngle)) * baseAcceleration,
			sin(glm::radians(enemyAngle)) * baseAcceleration,
			0
		);
	}
}

void PlayerGameObject::pickUp(shared_ptr<TreasureGameObject> treasure) {
	if (currentState != PlayerState::HURT && currentState != PlayerState::HURTING) {
		inventory->addToInventory(treasure);
	}
}

void PlayerGameObject::pickUp(shared_ptr<PowerupGameObject> powerup) {
	if (currentState != PlayerState::HURT && currentState != PlayerState::HURTING) {
		inventory->addToInventory(powerup);
	}
}

GLuint PlayerGameObject::playerTextureID = 0;
GLuint PlayerGameObject::playerMovingTextureID = 0;
GLuint PlayerGameObject::playerHurtTextureID = 0;
GLuint PlayerGameObject::playerBubblesTextureID = 0;
GLuint PlayerGameObject::playerFlippersTextureID = 0;

int PlayerGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname), GLuint* textures, int offset) {
	setFuncPtr(textures[offset + 0], "Assets\\player\\player-idle.png");
	setFuncPtr(textures[offset + 1], "Assets\\player\\player-swiming.png");
	setFuncPtr(textures[offset + 2], "Assets\\player\\player-hurt.png");
	setFuncPtr(textures[offset + 3], "Assets\\FX\\bubbles.png");
	setFuncPtr(textures[offset + 4], "Assets\\objects\\flippers.png");
	playerTextureID = textures[offset + 0];
	playerMovingTextureID = textures[offset + 1];
	playerHurtTextureID = textures[offset + 2];
	playerBubblesTextureID = textures[offset + 3];
	playerFlippersTextureID = textures[offset + 4];
	return offset + 5;
}